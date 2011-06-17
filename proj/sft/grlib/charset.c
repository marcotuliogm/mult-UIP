// Conjunto de caracteres 8x8.Necessario somar 0x20 no indice
static const unsigned char rom8x8_bits [] = {
/* 20 = ' ': */	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* 21 = '!': */	0x18,0x3c,0x3c,0x18,0x18,0x00,0x18,0x00,
/* 22 = '"': */	0x6c,0x6c,0x6c,0x00,0x00,0x00,0x00,0x00,
/* 23 = '#': */	0x6c,0x6c,0xfe,0x6c,0xfe,0x6c,0x6c,0x00,
/* 24 = '$': */	0x18,0x7e,0xc0,0x7c,0x06,0xfc,0x18,0x00,
/* 25 = '%': */	0x00,0xc6,0x0c,0x18,0x30,0x60,0xc6,0x00,
/* 26 = '&': */	0x38,0x6c,0x38,0x76,0xcc,0xcc,0x76,0x00,
/* 27 = ''': */	0x18,0x18,0x30,0x00,0x00,0x00,0x00,0x00,
/* 28 = '(': */	0x18,0x30,0x60,0x60,0x60,0x30,0x18,0x00,
/* 29 = ')': */	0x60,0x30,0x18,0x18,0x18,0x30,0x60,0x00,
/* 2A = '*': */	0x00,0xee,0x7c,0xfe,0x7c,0xee,0x00,0x00,
/* 2B = '+': */	0x00,0x18,0x18,0x7e,0x18,0x18,0x00,0x00,
/* 2C = ',': */	0x00,0x00,0x00,0x00,0x18,0x18,0x30,0x00,
/* 2D = '-': */	0x00,0x00,0x00,0xfe,0x00,0x00,0x00,0x00,
/* 2E = '.': */	0x00,0x00,0x00,0x00,0x00,0x38,0x38,0x00,
/* 2F = '/': */	0x06,0x0c,0x18,0x30,0x60,0xc0,0x80,0x00,
/* 30 = '0': */	0x7c,0xc6,0xce,0xde,0xf6,0xe6,0x7c,0x00,
/* 31 = '1': */	0x18,0x78,0x18,0x18,0x18,0x18,0x7e,0x00,
/* 32 = '2': */	0x7c,0xc6,0x0c,0x18,0x30,0x66,0xfe,0x00,
/* 33 = '3': */	0x7c,0xc6,0x06,0x3c,0x06,0xc6,0x7c,0x00,
/* 34 = '4': */	0x0c,0x1c,0x3c,0x6c,0xfe,0x0c,0x0c,0x00,
/* 35 = '5': */	0xfe,0xc0,0xfc,0x06,0x06,0xc6,0x7c,0x00,
/* 36 = '6': */	0x7c,0xc6,0xc0,0xfc,0xc6,0xc6,0x7c,0x00,
/* 37 = '7': */	0xfe,0xc6,0x06,0x0c,0x18,0x18,0x18,0x00,
/* 38 = '8': */	0x7c,0xc6,0xc6,0x7c,0xc6,0xc6,0x7c,0x00,
/* 39 = '9': */	0x7c,0xc6,0xc6,0x7e,0x06,0xc6,0x7c,0x00,
/* 3A = ':': */	0x00,0x1c,0x1c,0x00,0x00,0x1c,0x1c,0x00,
/* 3B = ';': */	0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x30,
/* 3C = '<': */	0x0c,0x18,0x30,0x60,0x30,0x18,0x0c,0x00,
/* 3D = '=': */	0x00,0x00,0xfe,0x00,0x00,0xfe,0x00,0x00,
/* 3E = '>': */	0x60,0x30,0x18,0x0c,0x18,0x30,0x60,0x00,
/* 3F = '?': */	0x7c,0xc6,0x06,0x0c,0x18,0x00,0x18,0x00,
/* 40 = '@': */	0x7c,0xc6,0xc6,0xde,0xdc,0xc0,0x7e,0x00,
/* 41 = 'A': */	0x38,0x6c,0xc6,0xc6,0xfe,0xc6,0xc6,0x00,
/* 42 = 'B': */	0xfc,0x66,0x66,0x7c,0x66,0x66,0xfc,0x00,
/* 43 = 'C': */	0x3c,0x66,0xc0,0xc0,0xc0,0x66,0x3c,0x00,
/* 44 = 'D': */	0xf8,0x6c,0x66,0x66,0x66,0x6c,0xf8,0x00,
/* 45 = 'E': */	0xfe,0xc2,0xc0,0xf8,0xc0,0xc2,0xfe,0x00,
/* 46 = 'F': */	0xfe,0x62,0x60,0x7c,0x60,0x60,0xf0,0x00,
/* 47 = 'G': */	0x7c,0xc6,0xc0,0xc0,0xde,0xc6,0x7c,0x00,
/* 48 = 'H': */	0xc6,0xc6,0xc6,0xfe,0xc6,0xc6,0xc6,0x00,
/* 49 = 'I': */	0x3c,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,
/* 4A = 'J': */	0x3c,0x18,0x18,0x18,0xd8,0xd8,0x70,0x00,
/* 4B = 'K': */	0xc6,0xcc,0xd8,0xf0,0xd8,0xcc,0xc6,0x00,
/* 4C = 'L': */	0xf0,0x60,0x60,0x60,0x60,0x62,0xfe,0x00,
/* 4D = 'M': */	0xc6,0xee,0xfe,0xd6,0xd6,0xc6,0xc6,0x00,
/* 4E = 'N': */	0xc6,0xe6,0xe6,0xf6,0xde,0xce,0xc6,0x00,
/* 4F = 'O': */	0x7c,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c,0x00,
/* 50 = 'P': */	0xfc,0x66,0x66,0x7c,0x60,0x60,0xf0,0x00,
/* 51 = 'Q': */	0x7c,0xc6,0xc6,0xc6,0xc6,0xd6,0x7c,0x06,
/* 52 = 'R': */	0xfc,0xc6,0xc6,0xfc,0xd8,0xcc,0xc6,0x00,
/* 53 = 'S': */	0x7c,0xc6,0xc0,0x7c,0x06,0xc6,0x7c,0x00,
/* 54 = 'T': */	0x7e,0x5a,0x18,0x18,0x18,0x18,0x3c,0x00,
/* 55 = 'U': */	0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c,0x00,
/* 56 = 'V': */	0xc6,0xc6,0xc6,0xc6,0x6c,0x38,0x10,0x00,
/* 57 = 'W': */	0xc6,0xc6,0xd6,0xd6,0xfe,0xee,0xc6,0x00,
/* 58 = 'X': */	0xc6,0x6c,0x38,0x38,0x38,0x6c,0xc6,0x00,
/* 59 = 'Y': */	0x66,0x66,0x66,0x3c,0x18,0x18,0x3c,0x00,
/* 5A = 'Z': */	0xfe,0x86,0x0c,0x18,0x30,0x62,0xfe,0x00,
/* 5B = '[': */	0x7c,0x60,0x60,0x60,0x60,0x60,0x7c,0x00,
/* 5C = '\': */	0xc0,0x60,0x30,0x18,0x0c,0x06,0x02,0x00,
/* 5D = ']': */	0x7c,0x0c,0x0c,0x0c,0x0c,0x0c,0x7c,0x00,
/* 5E = '^': */	0x10,0x38,0x6c,0xc6,0x00,0x00,0x00,0x00,
/* 5F = '_': */	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
/* 60 = '`': */	0x30,0x30,0x18,0x00,0x00,0x00,0x00,0x00,
/* 61 = 'a': */	0x00,0x00,0x78,0x0c,0x7c,0xcc,0x7e,0x00,
/* 62 = 'b': */	0xe0,0x60,0x7c,0x66,0x66,0x66,0xfc,0x00,
/* 63 = 'c': */	0x00,0x00,0x7c,0xc6,0xc0,0xc6,0x7c,0x00,
/* 64 = 'd': */	0x1c,0x0c,0x7c,0xcc,0xcc,0xcc,0x7e,0x00,
/* 65 = 'e': */	0x00,0x00,0x7c,0xc6,0xfe,0xc0,0x7c,0x00,
/* 66 = 'f': */	0x1c,0x36,0x30,0xfc,0x30,0x30,0x78,0x00,
/* 67 = 'g': */	0x00,0x00,0x76,0xce,0xc6,0x7e,0x06,0x7c,
/* 68 = 'h': */	0xe0,0x60,0x7c,0x66,0x66,0x66,0xe6,0x00,
/* 69 = 'i': */	0x18,0x00,0x38,0x18,0x18,0x18,0x3c,0x00,
/* 6A = 'j': */	0x0c,0x00,0x1c,0x0c,0x0c,0x0c,0xcc,0x78,
/* 6B = 'k': */	0xe0,0x60,0x66,0x6c,0x78,0x6c,0xe6,0x00,
/* 6C = 'l': */	0x18,0x18,0x18,0x18,0x18,0x18,0x1c,0x00,
/* 6D = 'm': */	0x00,0x00,0x6c,0xfe,0xd6,0xd6,0xc6,0x00,
/* 6E = 'n': */	0x00,0x00,0xdc,0x66,0x66,0x66,0x66,0x00,
/* 6F = 'o': */	0x00,0x00,0x7c,0xc6,0xc6,0xc6,0x7c,0x00,
/* 70 = 'p': */	0x00,0x00,0xdc,0x66,0x66,0x7c,0x60,0xf0,
/* 71 = 'q': */	0x00,0x00,0x76,0xcc,0xcc,0x7c,0x0c,0x1e,
/* 72 = 'r': */	0x00,0x00,0xdc,0x66,0x60,0x60,0xf0,0x00,
/* 73 = 's': */	0x00,0x00,0x7c,0xc0,0x7c,0x06,0x7c,0x00,
/* 74 = 't': */	0x30,0x30,0xfc,0x30,0x30,0x36,0x1c,0x00,
/* 75 = 'u': */	0x00,0x00,0xcc,0xcc,0xcc,0xcc,0x76,0x00,
/* 76 = 'v': */	0x00,0x00,0xc6,0xc6,0x6c,0x38,0x10,0x00,
/* 77 = 'w': */	0x00,0x00,0xc6,0xc6,0xd6,0xfe,0x6c,0x00,
/* 78 = 'x': */	0x00,0x00,0xc6,0x6c,0x38,0x6c,0xc6,0x00,
/* 79 = 'y': */	0x00,0x00,0xc6,0xc6,0xce,0x76,0x06,0x7c,
/* 7A = 'z': */	0x00,0x00,0xfc,0x98,0x30,0x64,0xfc,0x00,
/* 7B = '{': */	0x0e,0x18,0x18,0x70,0x18,0x18,0x0e,0x00,
/* 7C = '|': */	0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00,
/* 7D = '}': */	0x70,0x18,0x18,0x0e,0x18,0x18,0x70,0x00,
/* 7E = '~': */	0x76,0xdc,0x00,0x00,0x00,0x00,0x00,0x00,
/* 7F = '': */	0x00,0x10,0x38,0x38,0x6c,0x6c,0xfe,0x00
};