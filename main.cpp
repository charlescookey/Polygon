#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
using namespace std;
using namespace cv;

String NoteString[] = {"?","C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
void morphologicalyOpeningAndClosing(Mat);
void drawRectangleOverTheContours(vector<vector<Point>>,Rect,Mat);
int getOctaveNumber(int);
int getNote(int ,int);
void putNoteOverTheImage(vector<vector<Point>>,Rect,Mat,Point);

int main(int argc , char**argv){

    cv::VideoCapture cap;
    cap.open("adele.mp4");

    if(!cap.isOpened()){
        cout<<"image not found"<<endl;
        waitKey(0);
        return -1;
    }
    /*
    namedWindow("Control",WINDOW_NORMAL);
    int highH =179,lowH =0;
    int highS =255,lowS =0;
    int highV =255,lowV =0;

    createTrackbar("Low H", "Control",&lowH, 179);
    createTrackbar("High H","Control",&highH,179);
    createTrackbar("Low S", "Control",&lowS, 255);
    createTrackbar("High S","Control",&highS,255);
    createTrackbar("Low V", "Control",&lowV, 255);
    createTrackbar("High V","Control",&highV,255);
    */
    int BhighH =111,BlowH =95;
    int BhighS =255,BlowS =76;
    int BhighV =257,BlowV =44;

    int OhighH =39,OlowH =3;
    int OhighS =255,OlowS =63;
    int OhighV =255,OlowV =70;


    Mat dst;
    Mat img;
    Mat forT;
    Mat bForT;
    Mat oForT;
    Mat newd;
    vector<vector<Point>> bContours;
    vector<vector<Point>> oContours;
    while(true){
        bool mIsSuccess = cap.read(img);
        if(mIsSuccess == false){
            cout<<"end of frames"<<endl;
            break;
        }
        dst = Mat(img.clone(),Rect(29,493,1228,185));
        cvtColor(dst,forT,COLOR_BGR2HSV);

        inRange(forT,Scalar(BlowH,BlowS,BlowV),Scalar(BhighH,BhighS,BhighV),bForT);
        inRange(forT,Scalar(OlowH,OlowS,OlowV),Scalar(OhighH,OhighS,OhighV),oForT);

        morphologicalyOpeningAndClosing(bForT);
        morphologicalyOpeningAndClosing(oForT);


        //imshow("sub",dst);
        //imshow("Blue",bForT);
        //imshow("Orange",oForT);

        findContours(bForT,bContours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
        findContours(oForT,oContours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

        newd = img.clone();
        Rect ee;
        Point aa = Point(29,150);
        Point bb = Point(901,150);

        putNoteOverTheImage(oContours,ee,newd,aa);
        putNoteOverTheImage(bContours,ee,newd,bb);

        //drawRectangleOverTheContours(bContours,ee,newd);
        //drawRectangleOverTheContours(oContours,ee,newd);

        imshow("Result",newd);

        if(waitKey(30)==27)
            break;
    }
    waitKey(0);
    destroyAllWindows();
    return 0;
}
void morphologicalyOpeningAndClosing(Mat imageTo){
        erode(imageTo,imageTo,getStructuringElement(MORPH_RECT,Size(5,5)));
        dilate(imageTo,imageTo,getStructuringElement(MORPH_RECT,Size(5,5)));
        dilate(imageTo,imageTo,getStructuringElement(MORPH_RECT,Size(5,5)));
        erode(imageTo,imageTo,getStructuringElement(MORPH_RECT,Size(5,5)));
};
void drawRectangleOverTheContours(vector<vector<Point>> cont ,Rect rect,Mat theImg){
        for(int i = 0 ; i<cont.size() ;i ++ ){
            if(contourArea(cont[i]) > 1000 ){
                rect = boundingRect(cont[i]);
                int newx = rect.x + 29;
                int newy = rect.y + 493;
                rectangle(theImg,Point(newx , newy) ,Point(newx + rect.width , newy + rect.height),Scalar(0,255,0),3);
            }
        }
};

void putNoteOverTheImage(vector<vector<Point>> cont ,Rect rect,Mat theImg,Point a){
        for(int i = 0 ; i<cont.size() ;i ++ ){
            if(contourArea(cont[i]) > 1000 ){
                rect = boundingRect(cont[i]);
                int octaveNumber = getOctaveNumber(rect.x);
                int noteNumber = getNote(rect.x , rect.width);
                putText(theImg,NoteString[noteNumber] ,Point(a.x,(i > 0 ?(a.y + i*100):a.y)), FONT_HERSHEY_DUPLEX,3,Scalar(0,255,0),4);
                putText(theImg,to_string(octaveNumber),Point(a.x + 120,(i > 0 ?(a.y + i*100):a.y)),FONT_HERSHEY_DUPLEX,3,Scalar(0,255,0),4);

            }
        }
};

int getNote(int x,int z){//z = Rectangle lenght
    x = ((x-145)%252);

    if(x<15){
        return 1;
    }else if(x<50){
        if(z<25)
            return 2;
        return 3;
    }else if(x<85){
        if(z<25)
            return 4;
        return 5;
    }else if(x<120){
        return 6;
    }else if(x<160){
        if(z<25)
        return 7;
        return 8;
    }else if(x<195){
        if(z<25)
            return 9;
        return 10;
    }else if(x<230){
        if(z<25)
            return 11;
        return 12;
    }else{
        return 0;
};
};

int getOctaveNumber(int y){
    int x =y;
        if (x>140 && x <392){
            return 2;
        }else if (x>392 && x <644){
            return 3;
        }else if (x>644 && x <896){
            return 4;
        }else if (x>896 && x <1149){
            return 5;
        }else{
            return 1;
    };
};
