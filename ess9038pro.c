/*
 * Driver for Sabre ESS9038PRO DAC 
 *
 * Author: Alexey Spirkov <dev@alsp.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */


#include <linux/init.h>
#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/i2c.h>
#include <sound/soc.h>
#include <sound/pcm_params.h>
#include <sound/tlv.h>
#include <linux/gpio/consumer.h>


enum ESS9038PRO_Regs {
    ESS9038PRO_SYSTEM_REG = 0,
    ESS9038PRO_INPUT_SEL_REG = 1,
    ESS9038PRO_SERIAL_DATA_CONF_REG =2,
    ESS9038PRO_AUTOMUTE_TIME_REG = 4,
    ESS9038PRO_AUTOMUTE_LEVEL_REG = 5,
    ESS9038PRO_DEEMPFILTER_REG = 6,
    ESS9038PRO_FILTER_AND_MUTE_REG = 7,
    ESS9038PRO_GPIO12_CONF_REG = 8,
    ESS9038PRO_GPIO34_CONF_REG = 9,
    ESS9038PRO_MASTER_MODE_REG = 10,
    ESS9038PRO_SPDIF_MUX_REG = 11,
    ESS9038PRO_JITTER_ELIM1_REG = 12,
    ESS9038PRO_JITTER_ELIM2_REG = 13,
    ESS9038PRO_SOFT_START_REG = 14,
    ESS9038PRO_VOLUME_CONF_REG = 15,
    ESS9038PRO_VOLUME_CONTROL_1_REG = 16,
    ESS9038PRO_VOLUME_CONTROL_2_REG = 17,
    ESS9038PRO_VOLUME_CONTROL_3_REG = 18,
    ESS9038PRO_VOLUME_CONTROL_4_REG = 19,
    ESS9038PRO_VOLUME_CONTROL_5_REG = 20,
    ESS9038PRO_VOLUME_CONTROL_6_REG = 21,
    ESS9038PRO_VOLUME_CONTROL_7_REG = 22,
    ESS9038PRO_VOLUME_CONTROL_8_REG = 23,
    ESS9038PRO_MASTER_TRIM_0_REG = 24,
    ESS9038PRO_MASTER_TRIM_1_REG = 25,
    ESS9038PRO_MASTER_TRIM_2_REG = 26,
    ESS9038PRO_MASTER_TRIM_3_REG = 27,
    ESS9038PRO_THD_COMP_C2_0_REG = 28,
    ESS9038PRO_THD_COMP_C2_1_REG = 29,
    ESS9038PRO_THD_COMP_C3_0_REG = 30,
    ESS9038PRO_THD_COMP_C3_1_REG = 31,
    ESS9038PRO_PGM_FIR_RAM_ADDR_REG = 32,
    ESS9038PRO_PGM_FIR_DATA_0_REG = 33,
    ESS9038PRO_PGM_FIR_DATA_1_REG = 34,
    ESS9038PRO_PGM_FIR_DATA_2_REG = 35,
    ESS9038PRO_PGM_FIR_DATA_3_REG = 36,
    ESS9038PRO_PGM_FIR_CONF_REG = 37,
    ESS9038PRO_CHAN_MAP_1_2_REG = 38,
    ESS9038PRO_CHAN_MAP_3_4_REG = 39,
    ESS9038PRO_CHAN_MAP_5_6_REG = 40,
    ESS9038PRO_CHAN_MAP_7_8_REG = 41,
    ESS9038PRO_NCO_0_REG = 42,
    ESS9038PRO_NCO_1_REG = 43,
    ESS9038PRO_NCO_2_REG = 44,
    ESS9038PRO_NCO_3_REG = 45,
    ESS9038PRO_ADC_CONF_REG = 46,
    ESS9038PRO_ADC_FLT_FTR_SCALE_0_REG = 47,
    ESS9038PRO_ADC_FLT_FTR_SCALE_1_REG = 48,
    ESS9038PRO_ADC_FLT_FBQ_SCALE_0_REG = 49,
    ESS9038PRO_ADC_FLT_FBQ_SCALE_1_REG = 50,
    ESS9038PRO_ADC_FLT_FBQ_SCALE_2_REG = 51,
    ESS9038PRO_ADC_FLT_FBQ_SCALE_3_REG = 52,
    ESS9038PRO_DOP_BYPASS_REG = 54,
    ESS9038PRO_18DB_SCALE_REG = 62,
    ESS9038PRO_AUTO_CALIBRATE_REG = 63,
    ESS9038PRO_CHIP_ID_STATUS_REG = 64,
    ESS9038PRO_GPIO_READBACK_REG = 65,
    ESS9038PRO_PLL_STATUS_0_REG = 66,
    ESS9038PRO_PLL_STATUS_1_REG = 67,
    ESS9038PRO_PLL_STATUS_2_REG = 68,
    ESS9038PRO_PLL_STATUS_3_REG = 69,
    ESS9038PRO_SPDIF_STATUS_00_REG = 70,
    ESS9038PRO_SPDIF_STATUS_01_REG = 71,
    ESS9038PRO_SPDIF_STATUS_02_REG = 72,
    ESS9038PRO_SPDIF_STATUS_03_REG = 73,
    ESS9038PRO_SPDIF_STATUS_04_REG = 74,
    ESS9038PRO_SPDIF_STATUS_05_REG = 75,
    ESS9038PRO_SPDIF_STATUS_06_REG = 76,
    ESS9038PRO_SPDIF_STATUS_07_REG = 77,
    ESS9038PRO_SPDIF_STATUS_08_REG = 78,
    ESS9038PRO_SPDIF_STATUS_09_REG = 79,
    ESS9038PRO_SPDIF_STATUS_10_REG = 80,
    ESS9038PRO_SPDIF_STATUS_11_REG = 81,
    ESS9038PRO_SPDIF_STATUS_12_REG = 82,
    ESS9038PRO_SPDIF_STATUS_13_REG = 83,
    ESS9038PRO_SPDIF_STATUS_14_REG = 84,
    ESS9038PRO_SPDIF_STATUS_15_REG = 85,
    ESS9038PRO_SPDIF_STATUS_16_REG = 86,
    ESS9038PRO_SPDIF_STATUS_17_REG = 87,
    ESS9038PRO_SPDIF_STATUS_18_REG = 88,
    ESS9038PRO_SPDIF_STATUS_19_REG = 89,
    ESS9038PRO_SPDIF_STATUS_20_REG = 90,
    ESS9038PRO_SPDIF_STATUS_21_REG = 91,
    ESS9038PRO_SPDIF_STATUS_22_REG = 92,
    ESS9038PRO_SPDIF_STATUS_23_REG = 93,
    ESS9038PRO_INPUT_STATUS_REG = 100,
    ESS9038PRO_AGC1_GPIO2_0_REG = 110,
    ESS9038PRO_AGC1_GPIO2_1_REG = 111,
    ESS9038PRO_AGC1_GPIO2_2_REG = 112,
    ESS9038PRO_AGC2_GPIO1_0_REG = 113,
    ESS9038PRO_AGC2_GPIO1_1_REG = 114,
    ESS9038PRO_AGC2_GPIO1_2_REG = 115,
    ESS9038PRO_MAX_REG
};


/* ESS9038PRO Codec Private Data */
struct ess9038pro_priv {
	struct regmap *regmap;
    struct gpio_desc *reset_gpio;
	unsigned int fmt;
};

static bool ess9038pro_regs_writeable(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case ESS9038PRO_FILTER_AND_MUTE_REG:
    case ESS9038PRO_VOLUME_CONF_REG:
    case ESS9038PRO_VOLUME_CONTROL_1_REG:
    case ESS9038PRO_VOLUME_CONTROL_2_REG:
    case ESS9038PRO_VOLUME_CONTROL_3_REG:
    case ESS9038PRO_VOLUME_CONTROL_4_REG:
    case ESS9038PRO_VOLUME_CONTROL_5_REG:
    case ESS9038PRO_VOLUME_CONTROL_6_REG:
    case ESS9038PRO_VOLUME_CONTROL_7_REG:
    case ESS9038PRO_VOLUME_CONTROL_8_REG:
    case ESS9038PRO_CHAN_MAP_1_2_REG:
    case ESS9038PRO_CHAN_MAP_3_4_REG:
    case ESS9038PRO_CHAN_MAP_5_6_REG:
    case ESS9038PRO_CHAN_MAP_7_8_REG:
		return true;

	default:
		return false;
	}
}

static bool ess9038pro_regs_readable(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case ESS9038PRO_FILTER_AND_MUTE_REG:
	case ESS9038PRO_VOLUME_CONF_REG:
    case ESS9038PRO_VOLUME_CONTROL_1_REG:
    case ESS9038PRO_VOLUME_CONTROL_2_REG:
    case ESS9038PRO_VOLUME_CONTROL_3_REG:
    case ESS9038PRO_VOLUME_CONTROL_4_REG:
    case ESS9038PRO_VOLUME_CONTROL_5_REG:
    case ESS9038PRO_VOLUME_CONTROL_6_REG:
    case ESS9038PRO_VOLUME_CONTROL_7_REG:
    case ESS9038PRO_VOLUME_CONTROL_8_REG:
    case ESS9038PRO_INPUT_STATUS_REG:
    case ESS9038PRO_CHIP_ID_STATUS_REG:
	case ESS9038PRO_PLL_STATUS_0_REG:
	case ESS9038PRO_PLL_STATUS_1_REG:
	case ESS9038PRO_PLL_STATUS_2_REG:
	case ESS9038PRO_PLL_STATUS_3_REG:
    case ESS9038PRO_CHAN_MAP_1_2_REG:
    case ESS9038PRO_CHAN_MAP_3_4_REG:
    case ESS9038PRO_CHAN_MAP_5_6_REG:
    case ESS9038PRO_CHAN_MAP_7_8_REG:
        return true;

	default:
		return false;
	}
}

static bool ess9038pro_regs_volatile(struct device *dev, unsigned int reg)
{
	switch (reg) {
    case ESS9038PRO_INPUT_STATUS_REG:
	case ESS9038PRO_PLL_STATUS_0_REG:
	case ESS9038PRO_PLL_STATUS_1_REG:
	case ESS9038PRO_PLL_STATUS_2_REG:
	case ESS9038PRO_PLL_STATUS_3_REG:
		return true;

	default:
		return false;
	}
}

/* Volume Scale */
static const DECLARE_TLV_DB_SCALE(volume_tlv, -12750, 50, 1);


/* Filter Type */
static const char * const fir_filter_type_texts[] = {
	"fast roll-off, linear",
	"slow roll-off, linear",
	"fast roll-off, minimum",
	"slow roll-off, minimum",
	"apodizing fast roll-off, linear",
	"reserved",
	"hybrid fast roll-off, minimum",
	"brickwall"
};

static SOC_ENUM_SINGLE_DECL(ess9038pro_fir_filter_type_enum,
			ESS9038PRO_FILTER_AND_MUTE_REG, 5, fir_filter_type_texts);


static const char * const iir_filter_type_texts[] = {
	"1.0757fs 47.44K @ 44.1kHz",
	"1.1338fs 50K @ 44.1kHz",
	"1.3605fs 60K @ 44.1kHz",
	"1.5873fs 70K @ 44.1kHz",
};

static SOC_ENUM_SINGLE_DECL(ess9038pro_iir_filter_type_enum,
			ESS9038PRO_FILTER_AND_MUTE_REG, 1, iir_filter_type_texts);


/* Todo - move text to dts */
static const char * const input_sel_texts[] = {
	"Input1 - USB",
	"Input2 - Optical",
	"Input3 - SPDIF",
	"Input4 - Bluetooth"
};

static SOC_ENUM_SINGLE_DECL(ess9038pro_input_sel_enum,
				ESS9038PRO_CHAN_MAP_1_2_REG, 0, input_sel_texts);


static int ess9038pro_input_set(struct snd_kcontrol *kcontrol,
                          struct snd_ctl_elem_value *ucontrol)
{
        struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
        struct soc_enum *enum_ctrl = (struct soc_enum *)kcontrol->private_value;
        unsigned int reg = enum_ctrl->reg;
        unsigned int sel = ucontrol->value.enumerated.item[0];
        // todo change to mono here
        unsigned int bits = bits = ((sel*2 + 1)<<4) + (sel*2);

        snd_soc_component_update_bits(component, reg, 0xFF, bits);
        snd_soc_component_update_bits(component, reg+1, 0xFF, bits);
        snd_soc_component_update_bits(component, reg+2, 0xFF, bits);
        snd_soc_component_update_bits(component, reg+3, 0xFF, bits);

        return 0;
}

static int ess9038pro_input_get(struct snd_kcontrol *kcontrol,
                          struct snd_ctl_elem_value *ucontrol)
{
        struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
        struct soc_enum *enum_ctrl = (struct soc_enum *)kcontrol->private_value;
        unsigned int reg = enum_ctrl->reg;
        int val;

        val = snd_soc_component_read32(component, reg) & 0xff;

        ucontrol->value.enumerated.item[0] = (val&0xF)/2;

        return 0;
}

/* Control */
static const struct snd_kcontrol_new ess9038pro_controls[] = {
SOC_SINGLE_RANGE_TLV("Digital Playback Volume", ESS9038PRO_VOLUME_CONTROL_1_REG, 0, 0, 100, 1, volume_tlv),
SOC_ENUM("FIR Filter Type", ess9038pro_fir_filter_type_enum),
SOC_ENUM("IIR Filter Type", ess9038pro_iir_filter_type_enum),
SOC_ENUM_EXT("Input Select", ess9038pro_input_sel_enum, ess9038pro_input_get, ess9038pro_input_set),
};

static int ess9038pro_dai_startup(
		struct snd_pcm_substream *substream, struct snd_soc_dai *dai)
{
	struct snd_soc_component      *component = dai->component;
 	struct ess9038pro_priv *ess9038pro
 					= snd_soc_component_get_drvdata(component);

    // set default mode: stereo mode + ch1_vol to all + enable volume control
    snd_soc_component_update_bits(component, ESS9038PRO_VOLUME_CONF_REG, 0x7, 0x7); // todo change to defines

 	return 0;
}

static int ess9038pro_hw_params(
	struct snd_pcm_substream *substream, struct snd_pcm_hw_params *params,
	struct snd_soc_dai *dai)
{
	struct snd_soc_component      *component = dai->component;
	struct ess9038pro_priv *ess9038pro
					= snd_soc_component_get_drvdata(component);

    // ESS9038PRO able to autodetect all formats. 
    // so skip it
	
	return 0;
}

static int ess9038pro_set_fmt(struct snd_soc_dai *dai, unsigned int fmt)
{
	struct snd_soc_component      *component = dai->component;
	struct ess9038pro_priv *ess9038pro
					= snd_soc_component_get_drvdata(component);

    // ESS9038PRO able to autodetect all formats. 
    // so skip it

	ess9038pro->fmt = fmt;

	return 0;
}

static int ess9038pro_mute(struct snd_soc_dai *dai, int mute)
{
	struct snd_soc_component *component = dai->component;

	if (mute) {
		snd_soc_component_update_bits(component, ESS9038PRO_FILTER_AND_MUTE_REG, 0x01, 0x01);
	} else {
		snd_soc_component_update_bits(component, ESS9038PRO_FILTER_AND_MUTE_REG, 0x01, 0x00);
	}

	return 0;
}


static const struct snd_soc_dai_ops ess9038pro_dai_ops = {
	.startup      = ess9038pro_dai_startup,
	.hw_params    = ess9038pro_hw_params,
	.set_fmt      = ess9038pro_set_fmt,
	.digital_mute = ess9038pro_mute,
};

static struct snd_soc_dai_driver ess9038pro_dai = {
	.name = "ess9038pro-dai",
	.playback = {
		.stream_name  = "Playback",
		.channels_min = 2,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_CONTINUOUS,
		.rate_min = 8000,
		.rate_max = 1536000,
		.formats  = SNDRV_PCM_FMTBIT_S16_LE
				| SNDRV_PCM_FMTBIT_S32_LE
                | SNDRV_PCM_FMTBIT_S32_BE,
	},
	.ops = &ess9038pro_dai_ops,
};

static struct snd_soc_component_driver ess9038pro_driver = {
		.controls         = ess9038pro_controls,
		.num_controls     = ARRAY_SIZE(ess9038pro_controls),
};


static const struct regmap_config ess9038pro_regmap = {
	.reg_bits         = 8,
	.val_bits         = 8,
	.max_register     = ESS9038PRO_MAX_REG,

	.num_reg_defaults = 0,

	.writeable_reg    = ess9038pro_regs_writeable,
	.readable_reg     = ess9038pro_regs_readable,
	.volatile_reg     = ess9038pro_regs_volatile,

	.cache_type       = REGCACHE_RBTREE,
};

static int ess9038pro_probe(struct device *dev, struct regmap *regmap)
{
	struct ess9038pro_priv *ess9038pro;
	int ret;

	ess9038pro = devm_kzalloc(dev, sizeof(*ess9038pro), GFP_KERNEL);
	if (!ess9038pro) {
		dev_err(dev, "devm_kzalloc");
		return (-ENOMEM);
	}

	ess9038pro->regmap = regmap;

    ess9038pro->reset_gpio = devm_gpiod_get_optional(dev, "reset-gpio",
                                                    GPIOD_OUT_LOW);
    if (IS_ERR(ess9038pro->reset_gpio))
            return PTR_ERR(ess9038pro->reset_gpio);

    // remove reset
    gpiod_set_value_cansleep(ess9038pro->reset_gpio, 1);

    // todo check ID

	dev_set_drvdata(dev, ess9038pro);

	ret = snd_soc_register_component(dev,
			&ess9038pro_driver, &ess9038pro_dai, 1);
	if (ret != 0) {
		dev_err(dev, "Failed to register CODEC: %d\n", ret);
		return ret;
	}

	return 0;
}

static void ess9038pro_remove(struct device *dev)
{
	snd_soc_unregister_component(dev);
}

static int ess9038pro_i2c_probe(
		struct i2c_client *i2c, const struct i2c_device_id *id)
{
	struct regmap *regmap;

	regmap = devm_regmap_init_i2c(i2c, &ess9038pro_regmap);
	if (IS_ERR(regmap)) {
		return PTR_ERR(regmap);
	}

	return ess9038pro_probe(&i2c->dev, regmap);
}

static int ess9038pro_i2c_remove(struct i2c_client *i2c)
{
	ess9038pro_remove(&i2c->dev);

	return 0;
}

static const struct i2c_device_id ess9038pro_i2c_id[] = {
	{ "ess9038pro", },
	{ }
};
MODULE_DEVICE_TABLE(i2c, ess9038pro_i2c_id);

static const struct of_device_id ess9038pro_of_match[] = {
	{ .compatible = "sabre,ess9038pro", },
	{ }
};
MODULE_DEVICE_TABLE(of, ess9038pro_of_match);

static struct i2c_driver ess9038pro_i2c_driver = {
	.driver = {
		.name           = "ess9038pro-i2c",
		.owner          = THIS_MODULE,
		.of_match_table = of_match_ptr(ess9038pro_of_match),
	},
	.probe    = ess9038pro_i2c_probe,
	.remove   = ess9038pro_i2c_remove,
	.id_table = ess9038pro_i2c_id,
};
module_i2c_driver(ess9038pro_i2c_driver);


MODULE_DESCRIPTION("ESS9038PRO codec driver");
MODULE_AUTHOR("Alexey Spirkov <dev@alsp.net>");
MODULE_LICENSE("GPL");