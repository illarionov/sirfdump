include $(TOPDIR)/rules.mk

PKG_NAME:=sirfdump
PKG_VERSION:=0.2-14
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/littlesavage-sirfdump-af20e36
PKG_SOURCE:=littlesavage-sirfdump-v0.2-14-gaf20e36.zip
PKG_SOURCE_URL:=https://github.com/littlesavage/sirfdump/zipball/af20e364c61937279b2f8a1dcac410841f0c9771
PKG_MD5SUM:=ff98483f2200eb4f23f7507a6de4436d
PKG_CAT:=unzip

include $(INCLUDE_DIR)/package.mk

define Package/sirfdump
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Sirf binary logger
  URL:=https://github.com/littlesavage/sirfdump
endef

define Package/sirfdump/description
	Sirf binary logger
endef

define Package/sirfdump/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sirfdump $(1)/usr/bin/
endef

$(eval $(call BuildPackage,sirfdump))
