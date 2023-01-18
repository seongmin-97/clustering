#pragma warning(disable:4996)

#include"readImg.h"
#include"clustering.h"

int main(void)
{
	Image image;

	image = read_file();
	Img2Block(&image);
	clustering(&image);

	destroy_image(&image);

	return 0;
}