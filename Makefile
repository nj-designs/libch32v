APP_NAME ?= led-flash

DEVICE ?= ch32v203g6u6

APP_DIR := app/$(APP_NAME)

LIBCH32V_DIR = $(PWD)

include $(LIBCH32V_DIR)/module.mk
