#include "stdafx.h"
#include "sirfdump0.h"
#include "sirfdump.h"
#include "wince.h"

extern "C" unsigned output_dump_use_gsw230_byte_order;

struct ctx_t {

    TCHAR infile[MAX_PATH];
    TCHAR outfile[MAX_PATH];
    t_output_type output_type;
    unsigned gsw230_byte_order;

    HANDLE hfile;
    DWORD fileSize;
    DWORD filePointer;
    HWND hProgressBarWnd;
    unsigned head, tail;
    int last_errno;
    uint8_t buf[32768];

    FILE *outfh;

    dumpf_t *dump_f;
    void *user_ctx;
};

static volatile BOOL       g_bCancelConvert;   // Cancel convert

static struct ctx_t *init_ctx()
{
    struct ctx_t *ctx;

    ctx = (struct ctx_t *)malloc(sizeof(*ctx));

    if (ctx == NULL) {
        return NULL;
    }
    ctx->infile[0] = ctx->outfile[0] = '\0';
    ctx->output_type = OUTPUT_NMEA;
    ctx->gsw230_byte_order = 0;
    ctx->hfile = INVALID_HANDLE_VALUE;
    ctx->head = ctx->tail = 0;
    ctx->last_errno = 0;
    ctx->outfh = NULL;
    ctx->user_ctx = NULL;

    return ctx;
}

static void free_ctx(struct ctx_t *ctx)
{
    if (ctx == NULL)
        return;
    if (ctx->hfile != INVALID_HANDLE_VALUE)
        CloseHandle(ctx->hfile);
    if (ctx->outfh)
        fclose(ctx->outfh);
    free(ctx);
}

static DWORD read_data(struct ctx_t *ctx)
{
    DWORD l;

    if (ctx->tail == sizeof(ctx->buf)) {
        memmove(ctx->buf, &ctx->buf[ctx->head],
            ctx->tail - ctx->head);
        ctx->tail = ctx->tail - ctx->head;
        ctx->head=0;
    }

    if (ReadFile(ctx->hfile, &ctx->buf[ctx->tail], sizeof(ctx->buf) - ctx->tail,
        &l, NULL) == 0) {
            ctx->last_errno = GetLastError();
    }else {
        ctx->tail = ctx->tail+l;
        ctx->filePointer += l;

        if (ctx->fileSize != 0) {
            SendMessage(ctx->hProgressBarWnd, PBM_SETPOS, 
                (int)(100.0 * ctx->filePointer / ctx->fileSize), NULL);
        }

    }

    return l;
}

void *readpkt(struct ctx_t *ctx, struct transport_msg_t *res_msg)
{
    unsigned start_seq_found;
    unsigned payload_length;
    unsigned checksum;
    unsigned garbage_bytes;
    unsigned p0;
    size_t l;
    uint8_t *res;

    garbage_bytes = 0;
    start_seq_found=0;
    payload_length=0;

    for (;;) {
        while (ctx->tail - ctx->head < 8) {
            l = read_data(ctx);
            if (l <= 0)
                return NULL;
            if (g_bCancelConvert)
                return NULL;
        }

        /* search for start sequence  */
        while (ctx->head < ctx->tail-1) {
            if ((ctx->buf[ctx->head] == 0xa0)
                && (ctx->buf[ctx->head+1] == 0xa2)) {
                    start_seq_found=1;
                    break;
            }else {
                garbage_bytes++;
                ctx->head++;
            }
        }
        if (!start_seq_found)
            continue;

        while (ctx->tail - ctx->head < 6) {
            l = read_data(ctx);
            if (l <= 0)
                return NULL;
            if (g_bCancelConvert)
                return NULL;
        }

        /* get payload length  */
        payload_length = (0xff00 & (ctx->buf[ctx->head+2] << 8))
            | (0xff & ctx->buf[ctx->head+3]);
        if (payload_length >= 1023) {
            ctx->head++;
            continue;
        }

        /* load payload data */
        p0 = 2+2+payload_length;
        while (ctx->tail - ctx->head < p0+2+2) {
            l = read_data(ctx);
            if (l <= 0)
                return NULL;
            if (g_bCancelConvert)
                return NULL;
        }

        /* checksum  */
        checksum = (0xff00 & (ctx->buf[ctx->head+p0] << 8))
            | (0xff & ctx->buf[ctx->head+p0+1]);

        /* end sequence  */
        if (ctx->buf[ctx->head+p0+2] != 0xb0
            || (ctx->buf[ctx->head+p0+3] != 0xb3)) {
                ctx->head++;
                continue;
        }
        break;
    } /* for(;;)  */

    res = &ctx->buf[ctx->head];
    ctx->head = ctx->head + 2+2+payload_length+2+2;
    if (ctx->head == ctx->tail) {
        ctx->head = ctx->tail = 0;
    }

    if (res_msg) {
        res_msg->payload = &res[4];
        res_msg->payload_length = payload_length;
        res_msg->checksum = checksum;
        res_msg->skipped_bytes = garbage_bytes;
    }

    return res;
}


void cancelConvert()
{
    g_bCancelConvert = true;
}

DWORD ConvertThread(LPVOID hDlg)
{
    HWND ProgressBarWnd;
    HWND ConvertBtnHwnd;
    HWND CancelBtnHwnd;
    struct ctx_t *ctx;
    const TCHAR *ErrMsg;
    int length;
    void *pkt;
    struct transport_msg_t msg;

    ProgressBarWnd = GetDlgItem((HWND)hDlg, IDC_PROGRESS1);
    ConvertBtnHwnd = GetDlgItem((HWND)hDlg, IDC_CONVERT_BUTTON);
    CancelBtnHwnd = GetDlgItem((HWND)hDlg, IDC_CANCEL_BUTTON);

    g_bCancelConvert = FALSE;
    EnableWindow(ConvertBtnHwnd, FALSE);
    EnableWindow(CancelBtnHwnd, TRUE);

    ErrMsg = NULL;
    ctx = init_ctx();
    if (ctx == NULL) {
        ErrMsg = TEXT("malloc() error");
        goto endProcessing;
    }

    ctx->hProgressBarWnd = ProgressBarWnd;
    ctx->gsw230_byte_order = IsDlgButtonChecked((HWND)hDlg,
        IDC_ALTERNATE_BYTE_ORDER_CHECKBOX);

    ctx->output_type = GetSelectedOutputType((HWND)hDlg);

    length = GetDlgItemText((HWND)hDlg, IDC_INPUT_FILE_EDITBOX, ctx->infile, MAX_PATH);
    if (length <= 0) {
        ErrMsg = TEXT("Input file error");
        goto endProcessing;
    }

    length = GetDlgItemText((HWND)hDlg, IDC_OUTPUT_FILE_EDITBOX, ctx->outfile, MAX_PATH);
    if (length <= 0) {
        ErrMsg = TEXT("Output file error");
        goto endProcessing;
    }

    ctx->hfile = CreateFile(ctx->infile,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (ctx->hfile == INVALID_HANDLE_VALUE) {
        ErrMsg = TEXT("Could not open input file");
        goto endProcessing;
    }

    ctx->fileSize =  GetFileSize(ctx->hfile, NULL);
    ctx->filePointer = 0;

    ctx->outfh = _tfopen(ctx->outfile,TEXT("wb"));
    if (ctx->outfh == NULL) {
        ErrMsg = TEXT("Could not create output file");
        goto endProcessing;
    }

    switch (ctx->output_type) {
        case OUTPUT_NMEA:
            ctx->dump_f = &output_nmea;
            break;
        case OUTPUT_RINEX:
            ctx->dump_f = &output_rinex;
            ctx->user_ctx = new_rinex_ctx(0, NULL, ctx->gsw230_byte_order);
            if (ctx->user_ctx == NULL) {
                ErrMsg=TEXT("malloc() error");
                goto endProcessing;
            }
            break;
        case OUTPUT_RINEX_NAV:
            ctx->dump_f = &output_rinex_nav;
            ctx->user_ctx = new_rinex_nav_ctx(NULL, NULL);
            if (ctx->user_ctx == NULL) {
                ErrMsg=TEXT("malloc() error");
                goto endProcessing;
            }
            break;
        case OUTPUT_RTCM:
            ctx->dump_f = &output_rtcm;
            ctx->user_ctx = new_rtcm_ctx(NULL, NULL, ctx->gsw230_byte_order);
            if (ctx->user_ctx == NULL) {
                ErrMsg=TEXT("malloc() error");
                goto endProcessing;
            }
            break;
        case OUTPUT_DUMP:
        default:
            output_dump_use_gsw230_byte_order = ctx->gsw230_byte_order;
            ctx->dump_f = &output_dump;
            break;
    }


    while ( (pkt = readpkt(ctx, &msg)) != NULL ) {
        ctx->dump_f(&msg, ctx->outfh, ctx->user_ctx);
        if (g_bCancelConvert)
            break;
    }

    if (!g_bCancelConvert) {
        SendMessage(ProgressBarWnd, PBM_SETPOS, 0, NULL);
    }

endProcessing:

    if (ctx != NULL) {
        switch (ctx->output_type) {
            case OUTPUT_RINEX:
                free_rinex_ctx(ctx->user_ctx);
                break;
            case OUTPUT_RINEX_NAV:
                free_rinex_nav_ctx(ctx->user_ctx);
                break;
            case OUTPUT_RTCM:
                free_rtcm_ctx(ctx->user_ctx);
                break;
            default:
                break;
        }

        free_ctx(ctx);
    }

    if (ErrMsg != NULL) {
        MessageBox((HWND)hDlg, ErrMsg, NULL, MB_OK | MB_ICONERROR);
    }

    g_bCancelConvert = FALSE;
    EnableWindow(ConvertBtnHwnd, TRUE);
    EnableWindow(CancelBtnHwnd, FALSE);

    return 0;
}
