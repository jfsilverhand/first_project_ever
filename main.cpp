#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <graphics.h>
#include <string.h>
#include <curl/curl.h>
#include <math.h>
#include <ctype.h>
#define USE_LOCAL_FILE 1


// memory space for data storage
struct MemoryStruct {
    char *memory;
    size_t size;
};

// this function stores data when you get data
size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        printf("insufficent memory.\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

struct Point {
    int x;
    int y;
};


// calculates polynom's are with Shoelace formula

double calculatePolygonArea(struct Point points[], int numPoints) {
    double area = 0.0;

    for (int i = 0; i < numPoints - 1; i++) {
        area += (points[i].x * points[i + 1].y) - (points[i + 1].x * points[i].y);
    }

    area += (points[numPoints - 1].x * points[0].y) - (points[0].x * points[numPoints - 1].y);

    area = 0.5 * fabs(area);

    return area;
}


static int load_txt_into_memory(const char* local_path, struct MemoryStruct* chunk) {
#if USE_LOCAL_FILE
    // read it from local
    FILE* fp = fopen(local_path, "rb");
    if (!fp) {
        perror(local_path);
        return 1;
    }
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    rewind(fp);

    char* buf = (char*)realloc(chunk->memory, (size_t)sz + 1);
    if (!buf) {
        fclose(fp);
        fprintf(stderr, "insufficient memory.\n");
        return 1;
    }
    chunk->memory = buf;
    size_t rd = fread(chunk->memory, 1, (size_t)sz, fp);
    fclose(fp);

    chunk->memory[rd] = '\0';
    chunk->size = rd;
    return 0;
#else
    // take from internet (curl)
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl init failed\n");
        return 1;
    }
    curl_easy_setopt(curl, CURLOPT_URL, "https://raw.githubusercontent.com/<user>/<repo>/<branch>/prolab.txt");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)chunk);
    // if u need certificate:
    // curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return 1;
    }
    curl_easy_cleanup(curl);
    return 0;
#endif
}



int main(){
	
	int indx2=0;
	int indx3=0;
	int indx4=0;
    int out1[80];
    int out2[80];
    int out3[80];
    int out4[80];
    int index = 0;
    int num;
    int p2 = 0, p3 = 0, p4 = 0;
    int leftover = 0;
 	char data[300];
 	int linenumber;
 	
 	

	CURL *curl;
    CURLcode res;

    struct MemoryStruct chunk; // memory space for storing data

    chunk.memory = (char *)malloc(1);  // at first memory is empty
    chunk.size = 0;


	if (load_txt_into_memory("prolab.txt", &chunk) != 0) {
    	return 1;
	}


    if (chunk.size > 0) {
    	strcpy(data, chunk.memory); // leave a space to insert a null character
    	
	} else {
    // if chunk.memory is empty or its size is zero, clear the data array 
    	data[0] = '\0';
	}

	
	
	int maxline = 0;
	for(int i=0; data[i] != '\0'; i++){
		
		if(data[i] == '\n'){
			maxline++;
		}
	}
	maxline += 1;
	
	double platformcost;
 	double drillcost;
 	
 	printf("please enter a number for a line that you want to choose: \n");
    scanf("%d", &linenumber);
    printf("enter drilling cost (the value of unit drilling cost should be between 1 and 10): \n");
	scanf("%lf", &drillcost);
	printf("enter platform cost (the value of unit platform cost should be between 1 and 10): \n");
	scanf("%lf",&platformcost);	

    // take the line selected by the user
    int selectedLineIndex = 0;
    for (int i = 0; data[i] != '\0'; i++) {
        if (data[i] == '\n') {
            selectedLineIndex++;
            if (selectedLineIndex == linenumber - 1) {
                selectedLineIndex = i + 1; //33
                break;
            }
        }
    }

    if (selectedLineIndex == 0 || linenumber > maxline ) {
        printf("indicated line couldnt found.\n");
        return 1;
    }

    // process selected line
    int endOfLineIndex = selectedLineIndex;
    while (data[endOfLineIndex] != '\n' && data[endOfLineIndex] != '\0') {
        endOfLineIndex++; //99
    }


    char selectedLine[300];
    strncpy(selectedLine, data + selectedLineIndex, endOfLineIndex - selectedLineIndex);
    selectedLine[endOfLineIndex - selectedLineIndex] = '\0';

    printf("selected line: %s\n", selectedLine);
    	
            
    for (int i = 0; selectedLine[i+1] != '\0'; i++) {
	    if (isdigit(selectedLine[i+1])) {
            if (isdigit(selectedLine[i + 2])) {
                num = (selectedLine[i+1] - '0') * 10 + (selectedLine[i + 2] - '0');
                out1[index] = num;
                index++;
                i++;
            }
            else {
                num = selectedLine[i+1] - '0';
                out1[index] = num;
                index++;
            }
        }
    }
    

	
	int numberOfShapes = 0;
	
	for(int i = 0; out1[i] != '\0'; i += 2) {
    	for(int j = i + 2; out1[j] != '\0'; j += 2) {
        	if(out1[i] == out1[j] && out1[i + 1] == out1[j + 1]) {
        		
            	numberOfShapes++;
        }
    }
}

if(numberOfShapes>=1){
	
    for (int i = 2; i < index; i += 2) {
        if (out1[0] == out1[i] && out1[1] == out1[i+1]) {
            for (int j = 0; j <= i+1; j++) {
                out2[p2] = out1[j];
                p2++;
            }
            leftover = i+2;
            break;
        }
    }
}

out2[p2] = '\0';

if(numberOfShapes>=2){

    for (int i = leftover+2; i < index; i += 2) {
        if (out1[leftover] == out1[i] && out1[leftover+1] == out1[i+1]) {
            for (int j = leftover; j <= i+1; j++) {
                out3[p3] = out1[j];
                p3++;
            }
            leftover = i+2;
            break;
        }
    }
}

out3[p3] = '\0';

 if(numberOfShapes>=3){
 
   for (int i = leftover+2; i < index; i += 2) {
        if (out1[leftover] == out1[i] && out1[leftover+1] == out1[i+1]) {
            for (int j = leftover; j <= i+1; j++) {
                out4[p4] = out1[j];
                p4++;
            }
            leftover = i + 2;
            break;  // we want to break after the first match
        }
    }
}

out4[p4] = '\0';

if(numberOfShapes>=1){
    printf("first shapes coordinates (out2):\n");
    for (int i = 0; i < p2; i++) {
        printf("%d ", out2[i]);
    }
}
    printf("\n\n");
if(numberOfShapes>=2){
    printf("second shapes coordinates (out3):\n");
    for (int i = 0; i < p3; i++) {
        printf("%d ", out3[i]);
    }
}
    printf("\n\n");
if(numberOfShapes>=3){
    printf("third shapes coordinates (out4):\n");
    for (int i = 0; i < p4; i++) {
        printf("%d ", out4[i]);
    }
}
    
 	
		 
	if(numberOfShapes>=1){
	for(int i = 0; out2[i] != '\0'; i++){
		indx2++;
	}
}
	if(numberOfShapes>=2){
	for(int i = 0; out3[i] != '\0'; i++){
		indx3++;	
	}
}
	if(numberOfShapes>=3){
	for(int i = 0; out4[i] != '\0'; i++){
		indx4++;	
	}
}
	//area = 1/2 * |(x_1 * y_2 - x_2 * y_1) + (x_2 * y_3 - x_3 * y_2) + ... + (x_{n-1} * y_n - x_n * y_{n-1}) + (x_n * y_1 - x_1 * y_n)|
	int side2 = indx2/2-1;
	int side3 = indx3/2-1;
	int side4 = indx4/2-1;
//----------------------------------------------------------------------------------------------------------------------------------------------------
    // for example, let's use the vertices of a triangle
    double area22;
    struct Point geo2[side2]; 
    if(numberOfShapes>=1){	
    for (int i = 0; i < side2; i++) {
    	geo2[i].x = out2[i * 2];     // assign the x coordinate
    	geo2[i].y = out2[i * 2 + 1]; // assign the y coordinate
	}

    int numPoints2 = side2;

    double area2 = calculatePolygonArea(geo2, numPoints2);
    printf("area of the shape 1: %.2lf\n\n", area2);
    
    double realreserve2 =  area2 * 10.0;
    
    area22 = area2;
	
}
	double area33;
    struct Point geo3[side3]; 
    if(numberOfShapes>=2){	
    for (int i = 0; i < side3; i++) {
    	geo3[i].x = out3[i * 2];     // assign the x coordinate
    	geo3[i].y = out3[i * 2 + 1]; // assign the y coordinate
	}
    
    int numPoints3 = side3;

    double area3 = calculatePolygonArea(geo3, numPoints3);
    printf("area of the shape 2: %.2lf\n\n", area3);
    
    double realreserve3 = area3 *10.0;
	
	area33 = area3;
	 
}
	double area44;
    struct Point geo4[side4]; 
    if(numberOfShapes>=3){	
    for (int i = 0; i < side4; i++) {
    	geo4[i].x = out4[i * 2];     // assign the x coordinate
    	geo4[i].y = out4[i * 2 + 1]; // assign the y coordinate
	}
    
    int numPoints4 = side4;

    double area4 = calculatePolygonArea(geo4, numPoints4);
    printf("area of the shape 3: %.2lf\n\n", area4);
    
    double realreserve4 = area4 * 10.0; 
    
    area44 = area4;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
	
 
	
	initwindow(1000, 1000);
	


    for (int i = 0; out2[i] != '\0'; i++) {
        out2[i] = out2[i] * 10;
    }
    for (int i = 0; out3[i] != '\0'; i++) {
        out3[i] = out3[i] * 10;
    }
    for (int i = 0; out4[i] != '\0'; i++) {
        out4[i] = out4[i] * 10;
    }

	for (int i = 0; i < 1000; i += 10) {
        line(0, i, 1000, i);
        line(i, 0, i, 1000);
    }
	
     if(numberOfShapes>=1){	
	char h2[20];
	
	double graphArea2 = area22;
	
	sprintf(h2,"%.1f",graphArea2);
	
	float xo2 = (out2[0] + out2[2])/2.0;
	float yo2 = (out2[1] + out2[3])/2.0;
	
	settextstyle(3,HORIZ_DIR,1);
	
	setfillstyle(1, 6);
	// fill and draw the first polygon (shape 1)	
	 fillpoly(indx2/2, out2);
    drawpoly(indx2/2, out2);
    delay(100);
	
	outtextxy(xo2,yo2,h2);
}

	if(numberOfShapes>=2){	
	char h3[20];
	
	double graphArea3 = area33;
	
	sprintf(h3,"%.1f",graphArea3);
	
	float xo3 = (out3[0] + out3[2])/2.0;
	float yo3 = (out3[1] + out3[3])/2.0;
	
	settextstyle(3,HORIZ_DIR,1);
	
	setfillstyle(1, 6);
	// fill and draw the second polygon (shape 2)	
	fillpoly(indx3/2, out3);
    drawpoly(indx3/2, out3);
    
    outtextxy(xo3,yo3,h3);
}
	if(numberOfShapes>=3){	
	char h4[20];
	
	double graphArea4 = area44;
	
	sprintf(h4,"%.1f",graphArea4);
	
	float xo4 = (out2[0] + out2[2])/2.0;
	float yo4 = (out2[1] + out2[3])/2.0;
	
	settextstyle(3,HORIZ_DIR,1);
	
	setfillstyle(1, 6);
	// fill and draw the third polygon (shape 3)
	fillpoly(indx4/2, out4);
    drawpoly(indx4/2, out4);
    
    outtextxy(xo4,yo4,h4);

}
	int a = getch();
	if(a == 13){
	cleardevice();
	}

	
if(numberOfShapes>=1){	

	setfillstyle(1, 6);
	// fill and draw the first polygon (shape 1)	
	fillpoly(indx2/2, out2);
    drawpoly(indx2/2, out2);
	
}

	if(numberOfShapes>=2){	

	setfillstyle(1, 6);
	// fill and draw the second polygon (shape 2)	
	fillpoly(indx3/2, out3);
    drawpoly(indx3/2, out3);
    
}
	if(numberOfShapes>=3){	
	
	setfillstyle(1, 6);
	// fill and draw the third polygon (shape 3)
	fillpoly(indx4/2, out4);
    drawpoly(indx4/2, out4);

}
	
	for (int i = 0; out2[i] != '\0'; i++) {
        out2[i] = out2[i] / 10;
    }
    for (int i = 0; out3[i] != '\0'; i++) {
        out3[i] = out3[i] / 10;
    }
    for (int i = 0; out4[i] != '\0'; i++) {
        out4[i] = out4[i] / 10;
    }


	double ratio;
	double ratioplus;
	double xx;
	double yy;
	double one = 1*0.5*0.5*0.5*0.5*0.5;
	double oneplus = 1*0.5*0.5*0.5*0.5*0.5;
	double up1 = 0.0;
	double up2 = 0.0;
	
	setcolor(15);

	

    for (int i = 0; i < 1000; i += 10) {
        line(0, i, 1000, i);
        line(i, 0, i, 1000);
    }
	
   	//out2 loop for shape 1
   	
    for(int i=0; i<=indx2-4; i+=2){
    	
    if(out2[i] <= out2[i+2] && out2[i+1] <= out2[i+3]){
    	
		xx = out2[i+2]-out2[i];
		yy = out2[i+3]-out2[i+1];
		
		if(xx == 0 || yy == 0){
			continue;
		}
		
		if(xx >= yy){

		ratio = xx/yy;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		if(xx <= yy){

		ratio = yy/xx;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		while(xx == yy){
			
			up1 = out2[i]+ratio;
			up2 = out2[i+1]+one;
			
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1>=out2[i+2] || up2>=out2[i+3]){
				break;
			}
		}
		
		while(xx > yy){
			
			up1 = out2[i]+ratio;
			up2 = out2[i+1]+one;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1>=out2[i+2] || up2>=out2[i+3]){
				break;
			}
			
		}
		
		while(yy > xx){
			
			up1 = out2[i]+ratio;
			up2 = out2[i+1]+one;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1>=out2[i+2] || up2>=out2[i+3]){
				break;
			}
			
		}
	}
		 ratio=0.0;
		 ratioplus=0.0;
		 xx=0.0;
		 yy=0.0;
		 one = 1*0.5*0.5*0.5*0.5*0.5;
		 oneplus = 1*0.5*0.5*0.5*0.5*0.5;
		 up1 = 0.0;
		 up2 = 0.0;
	
	if(out2[i] >= out2[i+2] && out2[i+1] <= out2[i+3]){

		xx = out2[i]-out2[i+2];
		yy = out2[i+3]-out2[i+1];
		
		if(xx == 0 || yy == 0){
			continue;
		}
		
		if(xx >= yy){

		ratio = xx/yy;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		if(xx <= yy){

		ratio = yy/xx;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		
		while(xx == yy){
			
			up1 = out2[i]-ratio;
			up2 = out2[i+1]+one;
			
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1<=out2[i+2] || up2>=out2[i+3]){
				break;
			}
		}
		while(xx > yy){
			
			up1 = out2[i]-ratio;
			up2 = out2[i+1]+one;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1<=out2[i+2] || up2>=out2[i+3]){
				break;
			}
			
		}
		
		while(yy > xx){
			
			up1 = out2[i]-one;
			up2 = out2[i+1]+ratio;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1<=out2[i+2] || up2>=out2[i+3]){
				break;
			}	
		}
	}
		 ratio=0.0;
		 ratioplus=0.0;
		 xx=0.0;
		 yy=0.0;
		 one = 1*0.5*0.5*0.5*0.5*0.5;
		 oneplus = 1*0.5*0.5*0.5*0.5*0.5;
		 up1 = 0.0;
		 up2 = 0.0;
		
	if(out2[i] >= out2[i+2] && out2[i+1] >= out2[i+3]){
		
		xx = out2[i]-out2[i+2];
		yy = out2[i+1]-out2[i+3];
		
		if(xx == 0 || yy == 0){
			continue;
		}
		
		if(xx >= yy){

		ratio = xx/yy;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		if(xx <= yy){

		ratio = yy/xx;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		while(xx == yy){
			
			up1 = out2[i]-ratio;
			up2 = out2[i+1]-one;

			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1<=out2[i+2] || up2<=out2[i+3]){
				break;
			}
		}
		
		while(xx > yy){
			
			up1 = out2[i]-ratio;
			up2 = out2[i+1]-one;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1<=out2[i+2] || up2<=out2[i+3]){
				break;
			}
			
		}
		
		while(yy > xx){
			
			up1 = out2[i]-ratio;
			up2 = out2[i+1]-one;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1<=out2[i+2] || up2<=out2[i+3]){
				break;
			}
			
		}
		
	}
	
		ratio=0.0;
		 ratioplus=0.0;
		 xx=0.0;
		 yy=0.0;
		 one = 1*0.5*0.5*0.5*0.5*0.5;
		 oneplus = 1*0.5*0.5*0.5*0.5*0.5;
		 up1 = 0.0;
		 up2 = 0.0;
	
	if(out2[i] <= out2[i+2] && out2[i+1] >= out2[i+3]){
		
		xx = out2[i+2]-out2[i];
		yy = out2[i+1]-out2[i+3];
		
		if(xx == 0 || yy == 0){
			continue;
		}
		
		if(xx >= yy){

		ratio = xx/yy;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		if(xx <= yy){

		ratio = yy/xx;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		while(xx == yy){
			
			up1 = out2[i]+one;
			up2 = out2[i+1]-ratio;

			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1>=out2[i+2] || up2<=out2[i+3]){
				break;
			}
		}
		
		while(xx > yy){
			
			up1 = out2[i]+one;
			up2 = out2[i+1]-ratio;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1>=out2[i+2] || up2<=out2[i+3]){
				break;
			}
			
		}
		
		while(yy > xx){
			
			up1 = out2[i]+one;
			up2 = out2[i+1]-ratio;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1>=out2[i+2] || up2<=out2[i+3]){
				break;
			}		
		}
	}	
}
		ratio=0.0;
		 ratioplus=0.0;
		 xx=0.0;
		 yy=0.0;
		 one = 1*0.5*0.5*0.5*0.5*0.5;
		 oneplus = 1*0.5*0.5*0.5*0.5*0.5;
		 up1 = 0.0;
		 up2 = 0.0;
		
		
		//out3 loop for shape 2
		for(int i=0; i<=indx3-4; i+=2){
    	
    if(out3[i] <= out3[i+2] && out3[i+1] <= out3[i+3]){
    	
		xx = out3[i+2]-out3[i];
		yy = out3[i+3]-out3[i+1];
		
		if(xx == 0 || yy == 0){
			continue;
		}
		
		if(xx >= yy){

		ratio = xx/yy;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		if(xx <= yy){

		ratio = yy/xx;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		while(xx == yy){
			
			up1 = out3[i]+ratio;
			up2 = out3[i+1]+one;
			
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1>=out3[i+2] || up2>=out3[i+3]){
				break;
			}
		}
		
		while(xx > yy){
			
			up1 = out3[i]+ratio;
			up2 = out3[i+1]+one;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1>=out3[i+2] || up2>=out3[i+3]){
				break;
			}
			
		}
		
		while(yy > xx){
			
			up1 = out3[i]+ratio;
			up2 = out3[i+1]+one;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1>=out3[i+2] || up2>=out3[i+3]){
				break;
			}
			
		}
	}
		 ratio=0.0;
		 ratioplus=0.0;
		 xx=0.0;
		 yy=0.0;
		 one = 1*0.5*0.5*0.5*0.5*0.5;
		 oneplus = 1*0.5*0.5*0.5*0.5*0.5;
		 up1 = 0.0;
		 up2 = 0.0;
	
	if(out3[i] >= out3[i+2] && out3[i+1] <= out3[i+3]){

		xx = out3[i]-out3[i+2];
		yy = out3[i+3]-out3[i+1];
		
		if(xx == 0 || yy == 0){
			continue;
		}
		
		if(xx >= yy){

		ratio = xx/yy;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		if(xx <= yy){

		ratio = yy/xx;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		
		while(xx == yy){
			
			up1 = out3[i]-ratio;
			up2 = out3[i+1]+one;
			
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1<=out3[i+2] || up2>=out3[i+3]){
				break;
			}
		}
		while(xx > yy){
			
			up1 = out3[i]-ratio;
			up2 = out3[i+1]+one;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1<=out3[i+2] || up2>=out3[i+3]){
				break;
			}
			
		}
		
		while(yy > xx){
			
			up1 = out3[i]-one;
			up2 = out3[i+1]+ratio;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1<=out3[i+2] || up2>=out3[i+3]){
				break;
			}	
		}
	}
		 ratio=0.0;
		 ratioplus=0.0;
		 xx=0.0;
		 yy=0.0;
		 one = 1*0.5*0.5*0.5*0.5*0.5;
		 oneplus = 1*0.5*0.5*0.5*0.5*0.5;
		 up1 = 0.0;
		 up2 = 0.0;
		
	if(out3[i] >= out3[i+2] && out3[i+1] >= out3[i+3]){
		
		xx = out3[i]-out3[i+2];
		yy = out3[i+1]-out3[i+3];
		
		if(xx == 0 || yy == 0){
			continue;
		}
		
		if(xx >= yy){

		ratio = xx/yy;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		if(xx <= yy){

		ratio = yy/xx;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		while(xx == yy){
			
			up1 = out3[i]-ratio;
			up2 = out3[i+1]-one;

			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1<=out3[i+2] || up2<=out3[i+3]){
				break;
			}
		}
		
		while(xx > yy){
			
			up1 = out3[i]-ratio;
			up2 = out3[i+1]-one;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1<=out3[i+2] || up2<=out3[i+3]){
				break;
			}
			
		}
		
		while(yy > xx){
			
			up1 = out3[i]-ratio;
			up2 = out3[i+1]-one;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1<=out3[i+2] || up2<=out3[i+3]){
				break;
			}
			
		}
		
	}
	
	
	if(out3[i] <= out3[i+2] && out3[i+1] >= out3[i+3]){
		
		xx = out3[i+2]-out3[i];
		yy = out3[i+1]-out3[i+3];
		
		if(xx == 0 || yy == 0){
			continue;
		}
		
		if(xx >= yy){

		ratio = xx/yy;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		if(xx <= yy){

		ratio = yy/xx;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		while(xx == yy){
			
			up1 = out3[i]+one;
			up2 = out3[i+1]-ratio;

			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1>=out3[i+2] || up2<=out3[i+3]){
				break;
			}
		}
		
		while(xx > yy){
			
			up1 = out3[i]+one;
			up2 = out3[i+1]-ratio;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1>=out3[i+2] || up2<=out3[i+3]){
				break;
			}
			
		}
		
		while(yy > xx){
			
			up1 = out3[i]+one;
			up2 = out3[i+1]-ratio;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1>=out3[i+2] || up2<=out3[i+3]){
				break;
			}		
		}
	}	
}
	 	 ratio=0.0;
		 ratioplus=0.0;
		 xx=0.0;
		 yy=0.0;
		 one = 1*0.5*0.5*0.5*0.5*0.5;
		 oneplus = 1*0.5*0.5*0.5*0.5*0.5;
		 up1 = 0.0;
		 up2 = 0.0;
		
		
		//out4 loop for shape 3
		for(int i=0; i<=indx4-4; i+=2){
    	
    if(out4[i] <= out4[i+2] && out4[i+1] <= out4[i+3]){
    	
		xx = out4[i+2]-out4[i];
		yy = out4[i+3]-out4[i+1];
		
		if(xx == 0 || yy == 0){
			continue;
		}
		
		if(xx >= yy){

		ratio = xx/yy;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		if(xx <= yy){

		ratio = yy/xx;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		while(xx == yy){
			
			up1 = out4[i]+ratio;
			up2 = out4[i+1]+one;
			
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1>=out4[i+2] || up2>=out4[i+3]){
				break;
			}
		}
		
		while(xx > yy){
			
			up1 = out4[i]+ratio;
			up2 = out4[i+1]+one;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1>=out4[i+2] || up2>=out4[i+3]){
				break;
			}
			
		}
		
		while(yy > xx){
			
			up1 = out4[i]+ratio;
			up2 = out4[i+1]+one;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1>=out4[i+2] || up2>=out4[i+3]){
				break;
			}
			
		}
	}
		 ratio=0.0;
		 ratioplus=0.0;
		 xx=0.0;
		 yy=0.0;
		 one = 1*0.5*0.5*0.5*0.5*0.5;
		 oneplus = 1*0.5*0.5*0.5*0.5*0.5;
		 up1 = 0.0;
		 up2 = 0.0;
	
	if(out4[i] >= out4[i+2] && out4[i+1] <= out4[i+3]){

		xx = out4[i]-out4[i+2];
		yy = out4[i+3]-out4[i+1];
		
		if(xx == 0 || yy == 0){
			continue;
		}
		
		if(xx >= yy){

		ratio = xx/yy;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		if(xx <= yy){

		ratio = yy/xx;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		
		while(xx == yy){
			
			up1 = out4[i]-ratio;
			up2 = out4[i+1]+one;
			
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1<=out4[i+2] || up2>=out4[i+3]){
				break;
			}
		}
		while(xx > yy){
			
			up1 = out4[i]-ratio;
			up2 = out4[i+1]+one;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1<=out4[i+2] || up2>=out4[i+3]){
				break;
			}
			
		}
		
		while(yy > xx){
			
			up1 = out4[i]-one;
			up2 = out4[i+1]+ratio;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1<=out4[i+2] || up2>=out4[i+3]){
				break;
			}	
		}
	}
		 ratio=0.0;
		 ratioplus=0.0;
		 xx=0.0;
		 yy=0.0;
		 one = 1*0.5*0.5*0.5*0.5*0.5;
		 oneplus = 1*0.5*0.5*0.5*0.5*0.5;
		 up1 = 0.0;
		 up2 = 0.0;
		
	if(out4[i] >= out4[i+2] && out4[i+1] >= out4[i+3]){
		
		xx = out4[i]-out4[i+2];
		yy = out4[i+1]-out4[i+3];
		
		if(xx == 0 || yy == 0){
			continue;
		}
		
		if(xx >= yy){

		ratio = xx/yy;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		if(xx <= yy){

		ratio = yy/xx;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		while(xx == yy){
			
			up1 = out4[i]-ratio;
			up2 = out4[i+1]-one;

			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1<=out4[i+2] || up2<=out4[i+3]){
				break;
			}
		}
		
		while(xx > yy){
			
			up1 = out4[i]-ratio;
			up2 = out4[i+1]-one;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1<=out4[i+2] || up2<=out4[i+3]){
				break;
			}
			
		}
		
		while(yy > xx){
			
			up1 = out4[i]-ratio;
			up2 = out4[i+1]-one;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1<=out4[i+2] || up2<=out4[i+3]){
				break;
			}
			
		}
		
	}
	
	
	if(out4[i] <= out4[i+2] && out4[i+1] >= out4[i+3]){
		
		xx = out4[i+2]-out4[i];
		yy = out4[i+1]-out4[i+3];
		
		if(xx == 0 || yy == 0){
			continue;
		}
		
		if(xx >= yy){

		ratio = xx/yy;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		if(xx <= yy){

		ratio = yy/xx;
		ratio = ratio*0.5*0.5*0.5*0.5*0.5;
		ratioplus = ratio;
		
		}
		
		while(xx == yy){
			
			up1 = out4[i]+one;
			up2 = out4[i+1]-ratio;

			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1>=out4[i+2] || up2<=out4[i+3]){
				break;
			}
		}
		
		while(xx > yy){
			
			up1 = out4[i]+one;
			up2 = out4[i+1]-ratio;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1>=out4[i+2] || up2<=out4[i+3]){
				break;
			}
			
		}
		
		while(yy > xx){
			
			up1 = out4[i]+one;
			up2 = out4[i+1]-ratio;
		
			int array[10] = {floor(up1)*10,floor(up2)*10,ceil(up1)*10,floor(up2)*10,ceil(up1)*10,ceil(up2)*10,floor(up1)*10,ceil(up2)*10,floor(up1)*10,floor(up2)*10};
			setfillstyle(1,BROWN);
			fillpoly(5,array);
			
			ratio = ratio + ratioplus;
			one = one + oneplus;
			
			if(up1>=out4[i+2] || up2<=out4[i+3]){
				break;
			}		
		}
	}	
}
	
		
		int coloredsquare = 0;

// scan squares and count painted ones
for (int x = 5; x < 1000; x += 5) {
    for (int y = 5; y < 1000; y += 5) {
        int pixelcolor = getpixel(x, y);

        if (pixelcolor == 6) {
            coloredsquare++;
        }
    }
}

printf("total drilling area: %d\n",coloredsquare);
double area_rezervs;
area_rezervs = coloredsquare * 10.0; // change variable as double!!!




double totaldrill = drillcost * area_rezervs;

printf("total price of the drilling: %f\n", totaldrill);

    	
	
	int wym = getch();
	if(wym == 13){
	
	int sqno16 = 0;
	int sqno8 = 0;	
	int sqno4 = 0;	
	int sqno2 = 0;
	int sqno1 = 0;			
	int ctrl2 = 0;	
	int ctrl3 = 0;	
	int ctrl4 = 0;	
	int ctrl5 = 0;
		
	//16x16 scanner
		
		for (int i = 5; i < 850; i += 10) {
    		for (int j = 5; j < 850; j += 10) {
    				
        		for (int x = 0; x < 16; x++) {
            		for (int y = 0; y < 16; y++) {
            			int a;
            			try {
                		int a = getpixel(j + x * 10, i + y * 10);
                	} catch (...){
                		a=-1;
					}
                			if(a == 6){
                				ctrl5++;
							}
            		}
        		}
        		if(ctrl5 == 256){
        			setfillstyle(SOLID_FILL, LIGHTGREEN);
    				bar(j-5,i-5,(j-5)+160,(i-5)+160);
    				sqno16++;
				}
   			}
		}
	
	//8x8 scanner
		
		for (int i = 5; i < 930; i += 10) {
    		for (int j = 5; j < 930; j += 10) {
    			
        		for (int x = 0; x < 8; x++) {
            		for (int y = 0; y < 8; y++) {
            			int a;
            			try {
                		int a = getpixel(j + x * 10, i + y * 10);
                	} catch (...){
                		a=-1;
					}
                			if(a == 6){
                				ctrl4++;
							}
            		}
        		}
        		if(ctrl4 == 64){
        			setfillstyle(SOLID_FILL, YELLOW);
    				bar(j-5,i-5,(j-5)+80,(i-5)+80);
    				sqno8++;
				}
   			}
		}
		
		//4x4 scanner
		for (int i = 5; i < 970; i += 10) {
    		for (int j = 5; j < 970; j += 10) {
    			
        		for (int x = 0; x < 4; x++) {
            		for (int y = 0; y < 4; y++) {
            			int b;
                		try {
						int b = getpixel(j + x * 10, i + y * 10);
					} catch(...){
						b=-1;
					}
                		if(b == 6){
                			ctrl3++;
						}
            	}
        	}
        	if(ctrl3 == 16){
        			setfillstyle(SOLID_FILL, RED);
    				bar(j-5,i-5,(j-5)+40,(i-5)+40);
    				sqno4++;
				}
    	}
	}
	//2x2 scanner
		for (int i = 5; i < 990; i += 10) {
			
			int topLeft;
			int topRight;
			int bottomLeft;
			int bottomRight;
			
   			 for (int j = 5; j < 990; j += 10) {
   			 	try{
        		int topLeft = getpixel(j, i);
        	} catch(...){
        		topLeft = -1;
			}
				try{
        		int topRight = getpixel(j+10, i);
        	} catch(...){
        		topRight = -1;
			}
				try{
        		int bottomLeft = getpixel(j, i+10);
        	} catch(...){
        		bottomLeft = -1;
			}
				try{
        		int bottomRight = getpixel(j + 10, i + 10);
        	} catch(...){
        		bottomRight = -1;
			}

        	if (topLeft == 6 && topRight == 6 && bottomLeft == 6 && bottomRight == 6) {
            	setfillstyle(SOLID_FILL, BLUE);
           		 bar(j - 5, i - 5, j + 75, i + 75);
           		 sqno2++;
        }
    }
}

		for(int i = 5; i<1000; i+=10){
			ctrl2 = 0;
			for(int j = 5; j<1000; j+=10){
				int a = getpixel(j, i);
				if(a == 6){
					ctrl2++;
				}
				if(ctrl2 == 1){
				bar(j - 5, i - 5, j + 5, i - 5);
				sqno1++;
			}
			
			}
		}
		
		double mmm = 1.0*(sqno16 + sqno8 + sqno4 + sqno2 + sqno1);
		printf("total platform to built: %d\n",mmm);
		double totalplatform = platformcost * (sqno16 + sqno8 + sqno4 + sqno2 + sqno1)*mmm;
		printf("total price of platforms: %f\n", totalplatform);
		printf("the profit: %f\n", totaldrill - totalplatform);
		}

		getch();
		closegraph();

		free(chunk.memory);
		#if !USE_LOCAL_FILE
		
		curl_easy_cleanup(curl);
		
		#endif
		
		return 0;
	} 

	

	

