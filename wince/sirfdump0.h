#pragma once
#include "resourceppc.h"


enum t_output_type {
    OUTPUT_RINEX,
    OUTPUT_RINEX_NAV,
    OUTPUT_NMEA,
    OUTPUT_RTCM,
    OUTPUT_DUMP
};

// sorfdump.cpp
enum t_output_type GetSelectedOutputType(HWND hDlg);

// converter.cpp
DWORD ConvertThread(LPVOID hDlg);
void cancelConvert();
