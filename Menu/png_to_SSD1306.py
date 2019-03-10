from PIL import Image


def convert(png_file):
    image = Image.open(png_file)
    rgb_im = image.convert('L')
    string = '''const uint8_t array[] = {'''
    data_array = []
    for i in range(int(128 * 64 / 8)):
        data_array.append(0)

    for x in range(0, 127):
        for y in range(0, 63):
            v = rgb_im.getpixel((x, y))
            if v == 255:
                data_array[int(y / 8) * 128 + x] |= (1 << (y % 8))

    for i in range(0, len(data_array)):
        string += hex(data_array[i]) + ", "

    string = string[:-2]
    string += "};"
    print(string)
    

if __name__ == '__main__':
    convert(r"C:\Users\Standardbenutzer\Documents\ESP8266\Projects\ESP8266\Menu\Menu_Parts\Ret_Button\Ret_Invert.png")
