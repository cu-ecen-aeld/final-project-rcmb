################################################################################
#
# sensors package
#
#################################################################################

SENSORS_VERSION = 1.0
SENSORS_SITE = $(BR2_EXTERNAL_app_external_PATH)/package/sensors/src
SENSORS_SITE_METHOD = local# Other methods like git,wget,scp,file etc. are also available.

define SENSORS_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
    endef

define SENSORS_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/sensor_test  $(TARGET_DIR)/usr/bin
    endef

$(eval $(generic-package))
