
#include <common.h>
#include <fdt_support.h>


DECLARE_GLOBAL_DATA_PTR;

static void sunxi_board_type(void)
{
	int node;
	int output_type, output_mode;

	printf("sunxi_board_type\n");

	node = fdt_path_offset(working_fdt,"boot_disp");
	if (node >= 0) 
	{
		/* getproc output_type, indicate which kind of device will be using */
		if (fdt_getprop_u32(working_fdt, node, "output_type", (uint32_t*)&output_type) < 0) {
			printf("fetch script data boot_disp.output_type fail\n");
		} else
			printf("boot_disp.output_type=%d\n", output_type);

		/* getproc output_mode, indicate which kind of device will be using */
		if (fdt_getprop_u32(working_fdt, node, "output_mode", (uint32_t*)&output_mode) < 0) {
			printf("fetch script data boot_disp.output_mode fail\n");
		} else
			printf("boot_disp.output_mode=%d\n", output_mode);
	}
	else {
		printf("get disp parm failed\n");
	}

	/* 
	 * type: 1-lcd  3-hdmi
	 * mode: 2-lcd7,480p  5-lcd5,720p  a-1080p
	 */

	if(output_type == 1){
		if(output_mode == 2){
			setenv("board_type", "lcd7");
		}
		if(output_mode == 5){
			setenv("board_type", "lcd5");
		}
	}
	else if(output_type == 3){
		if(output_mode == 2){
			setenv("board_type", "480p");
		}
		if(output_mode == 5){
			setenv("board_type", "720p");
		}
		if(output_mode == 10){
			setenv("board_type", "1080p");
		}
	}
}

int sunxi_board_model(void)
{
	char model[128] = {0};
	sprintf(model, CONFIG_SUNXI_BOARD_MODEL);

	printf("sunxi board model: %s\n", model);
	if(setenv("board_model", model))
	{
		printf("error:set variable [board_model] fail\n");
	}

	if(!strcmp(model, "bpi-m64"))
		sunxi_board_type();

	return 0;
}
