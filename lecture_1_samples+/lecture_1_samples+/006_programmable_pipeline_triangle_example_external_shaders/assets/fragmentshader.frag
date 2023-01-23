#version 330

in vec3 fColor;
out vec4 sColor;

uniform float column;
uniform float row;

uniform vec2 mousePos;

uniform float numColumns;
uniform float numRows;

uniform float lightStrength;

uniform mat4 modelMatrix;

void main (void) {
    sColor = vec4(fColor,1);

    sColor.x -= 0.5f;
    sColor.y -= 0.5f;

    sColor *= modelMatrix;

    sColor.x += 0.5f;
    sColor.y += 0.5f;
    
    #define x sColor.r
    #define y sColor.g


    //MEDIOCRE
    float sizeX = 1.0f / numColumns; //the width of a column  eg. theres 10 columns -> 1/10 = 0.1.. so every column needs a width of 0.1 for everything to work out
    float sizeY = 1.0f / numRows; //the height of a row 

    float columnIndex = 0;
    float rowIndex = 0;

    columnIndex = (x/sizeX);
    rowIndex = (y/sizeY);


    //AVERAGE
    float dist = distance(mousePos, gl_FragCoord.xy) * lightStrength;
    vec3 color = vec3(1/dist, 1/dist ,1/dist);

    if(rowIndex < 0)
    {
        rowIndex -= 1;
    }
    if(columnIndex < 0)
    {
        columnIndex -= 1;
    }
    if((int(rowIndex) + int(columnIndex)) % 2 == 0){
          sColor = vec4(color, 1);
    } else {
           sColor = vec4(0,0,0,1);
    }

    

}

