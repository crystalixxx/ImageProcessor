# ImageProcessor
Application to process filters to bmp format photos

You can use application by call utils/build/image_processor file according to this format:
./image_processor path_to_input_file path_to_output_file -filter1 param1 param2 ...


List of all filters
-
- **-crop** *width* *height* - Croping your BMP file to min(current_width, width) to horizontal and to
  min(current_height, height) to vertical
- **-gs** - Appling grayscale filter to your BMP file
- **-neg** - Appling negative filter to your BMP file (Make RGB of all pixels opposite to current)
- **-sharp** - Adding sharping to your image
- **-edge** *threshold* - Creating borders of your image according to threshold value
- **-blur** *sigma* - Appling blur filter to your BMP file according to sigma value
- **-pixellate** *pixel_size* - Pixellating your BMP image accoring to size pixel_size (Will make
  image bigger if pixel_size not divide height or width of image)

Examples of filters
-
![Crop filter example](https://github.com/crystalixxx/ImageProcessor/raw/main/test_script/data/lenna_crop.bmp)
![Neg filter example](https://github.com/crystalixxx/ImageProcessor/raw/main/test_script/data/lenna_pixellate.bmp)
![Blur filter example](https://github.com/crystalixxx/ImageProcessor/raw/main/test_script/data/lenna_blur.bmp)

Other
-
In test_script/data you can see tests BMP files which you can use to test your own custom filters and in other your aims

Also you can call utils/build/image_processor_test to make sure you don't broke some parts of application