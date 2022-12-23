#version 330

in vec3 fColor;
out vec4 sColor;

uniform float column;
uniform float row;

void main (void) {
    sColor = vec4(fColor,1);

    #define x fColor.r
    #define y fColor.g


    //MEDIOCRE
    float sizeX = 1.0f / column; //the width of a column  eg. theres 10 columns -> 1/10 = 0.1.. so every column needs a width of 0.1 for everything to work out
    float sizeY = 1.0f / row; //the height of a row 

    int columnIndex = 0;
    int rowIndex = 0;

    columnIndex = int(x/sizeX);
    rowIndex = int(y/sizeY);


    if((rowIndex + columnIndex) % 2 == 0){
          sColor = vec4(1,1,1,1);
    } else {
           sColor = vec4(0,0,0,1);
    }
}

