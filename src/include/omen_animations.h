// SPDX-License-Identifier: GPL-3
/*
 * HP OMEN RGB Keyboard Driver - Animation System
 *
 * RGB animation effects and control
 *
 * Author: alessandromrc
 */

#ifndef OMEN_ANIMATIONS_H
#define OMEN_ANIMATIONS_H

#include <linux/device.h>
#include <linux/types.h>
#include <linux/mutex.h>

#include "omen_zones.h"

/* Animation system constants */
#define ANIMATION_TIMER_INTERVAL_MS 50
#define ANIMATION_SPEED_MIN 1
#define ANIMATION_SPEED_MAX 10
#define ANIMATION_SPEED_DEFAULT 1

enum animation_mode {
	ANIMATION_STATIC = 0,
	ANIMATION_BREATHING,
	ANIMATION_RAINBOW,
	ANIMATION_WAVE,
	ANIMATION_PULSE,
	ANIMATION_CHASE,
	ANIMATION_SPARKLE,
	ANIMATION_CANDLE,
	ANIMATION_AURORA,
	ANIMATION_DISCO,
	ANIMATION_GRADIENT,
	ANIMATION_SPECTRUM,
	ANIMATION_COUNT
};

/* Spectrum animation constants
 *
 * Cycle length is SPECTRUM_CYCLE_BASE_MS / (speed + 1), which puts the two
 * slowest steps close together where the drift is most visible: 20s at
 * speed 1, 13.3s at speed 2, down to 3.6s at speed 10. Dividing by speed
 * alone, as the other modes do, would jump straight from 20s to 10s.
 */
#define SPECTRUM_CYCLE_BASE_MS 40000

/* Steps in one full turn of the wheel: 6 sectors of 256 */
#define SPECTRUM_WHEEL_STEPS 1536

/* Gradient animation constants */
#define GRADIENT_MAX_COLORS 16
#define GRADIENT_MAX_GROUPS 4

struct gradient_group {
	u8 zone_mask;
	u8 color_count;
	struct color_platform colors[GRADIENT_MAX_COLORS];
};

struct gradient_config {
	u8 group_count;
	struct gradient_group groups[GRADIENT_MAX_GROUPS];
};

/* External animation state */
extern enum animation_mode current_animation;
extern int animation_speed;
extern bool animation_active;
extern struct gradient_config gradient_cfg;
extern struct mutex gradient_cfg_mutex;

/* Device attributes for sysfs */
extern struct device_attribute animation_brightness_attr;
extern struct device_attribute animation_mode_attr;
extern struct device_attribute animation_speed_attr;
extern struct device_attribute gradient_config_attr;

/**
 * animation_init - Initialize animation system
 */
void animation_init(void);

/**
 * animation_cleanup - Clean up animation resources
 */
void animation_cleanup(void);

/**
 * animation_start - Start currently selected animation
 */
void animation_start(void);

/**
 * animation_stop - Stop current animation
 */
void animation_stop(void);

/**
 * animation_set_mode - Set animation mode
 * @mode: New animation mode
 */
void animation_set_mode(enum animation_mode mode);

/**
 * animation_get_mode - Get current animation mode
 *
 * Returns: Current animation mode
 */
enum animation_mode animation_get_mode(void);

/**
 * hsv_to_rgb - Convert HSV color to RGB
 * @h: Hue (0-360)
 * @s: Saturation (0-100)
 * @v: Value (0-100)
 * @rgb: Output RGB color
 */
void hsv_to_rgb(int h, int s, int v, struct color_platform *rgb);

#endif /* OMEN_ANIMATIONS_H */

