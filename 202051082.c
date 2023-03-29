#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

/*
    Harsh Mahalwar
    202051082
    Lab Assignment - 3
*/

uint8_t plain_text[16];
uint8_t key_final[11][16];
uint8_t key_initial[16];

const uint8_t round_constant_matrix[10] = { 
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

unsigned char sub_byte_matrix[16][16]=
{   {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
    {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
    {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
    {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
    {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
    {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
    {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
    {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
    {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
    {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
    {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
    {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
    {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
    {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
    {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
    {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
};


// This function is used to substitute the bytes in the state matrix with the values in the S-box
unsigned char InverseMixColumnHelper(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    while (b) {
        if (b & 1)
            p ^= a;
        if (a & 0x80)
            a = (a << 1) ^ 0x1b;
        else
            a <<= 1;
        b >>= 1;
    }

    return p;
}

// This function is used to perform the InverseMixColumn operation on the state matrix
void InverseMixColumn() {
    unsigned char a, b, c, d;
    for (int i = 0; i < 4; i++) {
        a = plain_text[i], b = plain_text[i + 4], c = plain_text[i + 8], d = plain_text[i + 12];
        plain_text[i] = InverseMixColumnHelper(a, 0xa5) ^ InverseMixColumnHelper(b, 0x07) ^ InverseMixColumnHelper(c, 0x1a) ^ InverseMixColumnHelper(d, 0x73);
        plain_text[i + 4] = InverseMixColumnHelper(a, 0x73) ^ InverseMixColumnHelper(b, 0xa5) ^ InverseMixColumnHelper(c, 0x07) ^ InverseMixColumnHelper(d, 0x1a);
        plain_text[i + 8] = InverseMixColumnHelper(a, 0x1a) ^ InverseMixColumnHelper(b, 0x73) ^ InverseMixColumnHelper(c, 0xa5) ^ InverseMixColumnHelper(d, 0x07);
        plain_text[i + 12] = InverseMixColumnHelper(a, 0x07) ^ InverseMixColumnHelper(b, 0x1a) ^ InverseMixColumnHelper(c, 0x73) ^ InverseMixColumnHelper(d, 0xa5);    
    }
}



// This function is used to perform the Left shift operation on the state matrix
void shiftLeft(unsigned char x[4][4], int i) {
    unsigned char t1=x[i][0];
    x[i][0]=x[i][1];
    x[i][1]=x[i][2];
    x[i][2]=x[i][3];
    x[i][3]=t1;
}

// This function is used to perform the ShiftRow operation on the state matrix
void shiftrow(unsigned char x[4][4]){
    for(int i = 0; i < 4; i++) {
        int t = i;
        while(t--) {
            shiftLeft(x, i);
        }
    }
}

// This function is used to perform the Right shift operation on the state matrix
void shiftRight(unsigned char x[4][4], int i) {
    unsigned char t1=x[i][3];
    x[i][3]=x[i][2];
    x[i][2]=x[i][1];
    x[i][1]=x[i][0];
    x[i][0]=t1;
}

// This function is used to perform the InverseShiftRow operation on the state matrix
void invshiftrow(unsigned char x[4][4]){
    for(int i = 0; i < 4; i++) {
        int t = i;
        while(t--) {
            shiftRight(x, i);
        }
    }
}

// This function is used to perform the MixColumn operation
unsigned char mixcoloumn(unsigned char a){
    unsigned char temp;
    temp = a << 1;
    if (a >> 7)
        temp = temp ^ 27;

    return temp;
}

// This function is used to perform the MixColumn operation on the state matrix
void mixcol(unsigned char b[4][4]){
    unsigned char a[4][4];
    for(int i = 0; i < 4; i++) 
        for(int j = 0; j < 4; j++) 
            a[i][j] = b[i][j];

    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            b[i][j]=a[i][j]^mixcoloumn(mixcoloumn(a[(i+1)%4][j]))^mixcoloumn(mixcoloumn(a[(i+2)%4][j]))^mixcoloumn(mixcoloumn(a[(i+3) % 4][j]))^a[(i+3)%4][j];

}

// This function is used to perform the subbyte operation on the state matrix
unsigned char subbyte(unsigned char a){
    int t1,t2;
    t1=a&15;
    t2=(a>>4);
    return(sub_byte_matrix[t2][t1]);
}

// This function is used to perform the Rotate word operation 
uint32_t RotateWord(uint32_t word)
{
    return ((word << 8) & 0xffffff00) | ((word >> 24) & 0x000000ff);
}

// This function is used to perform the SubByte operation on the state matrix
uint32_t SubWord(uint32_t word)
{
    uint8_t b0, b1, b2, b3;
    uint8_t indx1 = (word >> 24) & 0xff, indx2 = (word >> 16) & 0xff, indx3 = (word >> 8) & 0xff, indx4 = word & 0xff;      

    b0 = sub_byte_matrix[indx1 >> 4][indx1 & 0xf];
    b1 = sub_byte_matrix[indx2][indx2 & 0xf];
    b2 = sub_byte_matrix[indx3][indx3 & 0xf];
    b3 = sub_byte_matrix[indx4][indx4 & 0xf];

    return ((uint32_t)b0 << 24) | ((uint32_t)b1 << 16) | ((uint32_t)b2 << 8) | (uint32_t)b3;
}

// AES key_initial scheduling function 
void KeyScheduler(uint8_t *key_initial, uint32_t *word)
{
    int i, j;
    uint32_t temp;

    for (i = 0; i < 4; i++) 
        word[i] = ((uint32_t)key_initial[4*i] << 24) | ((uint32_t)key_initial[4*i+1] << 16) | ((uint32_t)key_initial[4*i+2] << 8) | (uint32_t)key_initial[4*i+3];

    for (i = 4; i < 44; i++) {
        temp = word[i-1];

        if (!i % 4) 
            temp = SubWord(RotateWord(temp)) ^ ((uint32_t)round_constant_matrix[i/4-1] << 24);

        word[i] = word[i-4] ^ temp;
    }
}

// This function is used to perform the GetKeys operation
void GetKeys() {
    
    uint32_t word[44];
    KeyScheduler(key_initial, word);

    
    for(int i = 0; i < 11; i++) 
        for(int j = 0; j < 4; j++) 
            key_final[i][4*j] = (word[4*i+j] >> 24) & 0xff, key_final[i][4*j+1] = (word[4*i+j] >> 16) & 0xff, key_final[i][4*j+2] = (word[4*i+j] >> 8) & 0xff, key_final[i][4*j+3] = word[4*i+j] & 0xff;

}

// This function is used to perform the Xor with Key operation, takes the key number as input
void XorwithKey(int Keynum) {
    for(int i = 0; i < 16; i++) 
        plain_text[i] = plain_text[i] ^ key_final[Keynum][i];
}


// This function is used to perform the AES encryption
void Encryption(){
    printf("Plain text is \n");
    for(int i = 0; i < 16; i++) 
        printf("%02x ", plain_text[i]);
    printf("\n\n\n");

    // Key scheduling
    GetKeys();
    
    // Rounds 1 - 9 for AES encryption
    for(int i = 0; i < 9; i++) {
        XorwithKey(i);

        unsigned char b[4][4];

        for(int i = 0; i < 4; i++) 
            for(int j = 0; j < 4; j++) 
                b[i][j] = subbyte(mixcoloumn(plain_text[(i * 4) + j]) ^ 1);

        shiftrow(b);
        mixcol(b);
        int cnt = 0;
        for(int i = 0; i < 4; i++) 
            for(int j = 0; j < 4; j++) 
                plain_text[cnt++] = b[i][j];

    }

    // Round 10
    XorwithKey(9);
    unsigned char b[4][4];
    for(int i = 0; i < 4; i++) 
        for(int j = 0; j < 4; j++) 
            b[i][j] = subbyte(mixcoloumn(plain_text[(i * 4) + j]) ^ 1);

    shiftrow(b);
    int cnt = 0;
    for(int i = 0; i < 4; i++) 
        for(int j = 0; j < 4; j++) 
            plain_text[cnt++] = b[i][j];

    
    XorwithKey(10);
    printf("Encrypted text is \n");
    for(int i = 0; i < 16; i++) 
        printf("%02x ", plain_text[i]);
    printf("\n\n\n");
}

// This function is used to perform the inverse shift row operation
unsigned char InvSubByte(unsigned char b) {
    unsigned char temp = b, temp2 =b, temp3 = b;
    
    for(int i = 0; i < 7; i++) 
        temp = mixcoloumn(temp);
    
    for(int i = 0; i < 3; i++) 
        temp2 = mixcoloumn(temp2);
    
    temp = temp ^ temp2;
    
    for(int i = 0; i < 2; i++) 
        temp3 = mixcoloumn(temp3);

    temp = temp ^ temp3;
    return temp ^ b;
}

// This function is used to retrieve the row and column of the inverse sub byte matrix
uint8_t findRowAndCol(uint8_t b) {
    for(int i = 0; i < 16; i++) 
        for(int j = 0; j < 16; j++) 
            if(sub_byte_matrix[i][j] == b) 
                return (i << 4) | j;

    return 0;
}

// This function is used to perform the AES decryption
void Decryption() {
    
    XorwithKey(10);

    unsigned char b[4][4];
    for(int i = 0; i < 4; i++) 
        for(int j = 0; j < 4; j++) 
            b[i][j] = plain_text[i * 4 + j];

    invshiftrow(b);

    for(int i = 0; i < 4; i++) 
        for(int j = 0; j < 4; j++) 
            b[i][j] = findRowAndCol(b[i][j]) ^ 1, b[i][j] = InvSubByte(b[i][j]);

    int cnt = 0;
    for(int i = 0; i < 4; i++) 
            for(int j = 0; j < 4; j++) 
                plain_text[cnt++] = b[i][j];

    XorwithKey(9);

    for(int i = 8; i >= 0; i--) {
        InverseMixColumn();
        unsigned char h[4][4];
        
        for(int i = 0; i < 4; i++) 
            for(int j = 0; j < 4; j++) 
                h[i][j] = plain_text[i * 4 + j];

        invshiftrow(h);

        for(int i = 0; i < 4; i++) 
            for(int j = 0; j < 4; j++) {
                h[i][j] = findRowAndCol(h[i][j]) ^ 1;
                h[i][j] = InvSubByte(h[i][j]);
            }

        int cnt = 0;
        for(int i = 0; i < 4; i++) {
                for(int j = 0; j < 4; j++) {
                    plain_text[cnt++] = h[i][j];
                }
        }
        XorwithKey(i);
        
    }
    printf("The Decrypted text is \n");
    for(int i = 0; i < 16; i++) 
        printf("%02x ", plain_text[i]);
    printf("\n\n\n");
}

void TakeInputs() {
    printf("Enter the plain text in hexadecimal\n");
    for(int i = 0; i < 16; i++) 
        scanf("%02x", &plain_text[i]);
    
    printf("\nEnter the key in hexadecimal\n");
    for(int i = 0; i < 16; i++) 
        scanf("%02x", &key_initial[i]);
    printf("\n\n\n");
}

// driver code
int main() {
    TakeInputs();
    Encryption();
    Decryption();
    return 0;
}