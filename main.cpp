//#include "matplotlibcpp.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <algorithm>
constexpr double M_PI = 3.14159265358979323846;

using std::vector;
using std::string;
using std::cout;


//A structure to contain Points for  x and y
struct Point {
    double x, y;

    //Default constructor
    Point() {
        x = 0;
        y = 0;
    }
    //Constructor
    Point(double _x, double _y) : x(_x), y(_y) {};

    //operator to check if two points are same
    bool operator==(Point const& b) {
        return x == b.x && y == b.y;
    }
    //to print points
    void print() {
        cout << x << " " << y << "\n";
    }
};

//Check if two points intersect
bool intersect(Point p1, Point p2, Point p3, Point p4 , Point &intersect) {

    // Check if none of the lines are of length 0
    //if ((p1.x == p2.x && p1.y == p2.y) || (p3.x == p4.x && p3.y == p4.y)) {
    if ((p1 == p2) || (p3 == p4)) {
        return false;
    }

    double denominator = ((p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y));

        // Lines are parallel
    if (denominator == 0) {
        return false;
    }

    double ua = ((p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x)) / denominator;
    double ub = ((p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x)) / denominator;

        // is the intersection along the segments
    if (ua < 0 || ua > 1 || ub < 0 || ub > 1) {
        return false;
    }

    // Return a object with the x and y coordinates of the intersection
    intersect.x = p1.x + ua * (p2.x - p1.x);
    intersect.y = p1.y + ua * (p2.y - p1.y);

    return true;
}

double angle(const Point& a, const Point& center) {
    return atan2(a.y - center.y, a.x - center.x) * 180 / M_PI;
}

// Define a simple structure to represent a polygon as a collection of points
class Polygon {

public:
    Polygon() {
        vertices.clear();
    }
    vector<Point> vertices;

    void add(Point a) {
        vertices.push_back(a);
    }

    int remove(Point a) {
        for (std::vector<Point>::iterator it = vertices.begin(); it != vertices.end();)
        {
            if (*it == a) {
                vertices.erase(it);
                return 0;
            }
        }
        return -1;
    }

    //void plot() {
    //    vector<int> xaxis{};
    //    vector<int> yaxis{};


    //    for (Point a : vertices) {
    //        xaxis.push_back(a.x);
    //        yaxis.push_back(a.y);
    //    }

    //    plt::plot(xaxis, yaxis);

    //}

    size_t size() const{
        return vertices.size();
    }

    bool contains(Point a) {
        for (Point vertex : vertices) {
            if (a == vertex)return true;
        }
        return false;
    }

    bool print(string name) {
        cout << "\nThe vertices contained in " << name<<": \n";
        for (Point vertex : vertices) {
            vertex.print();
        }
        return false;
    }
    Point findCentroid() {
        int x = 0;
        int y = 0;
        for (const Point& vertex : vertices) {
            x += vertex.x;
            y += vertex.y;
        }
        Point center;
        center.x = x / this->size();
        center.y = y / this->size();
        return center;
    }

    // Function to sort vertices based on their angle relative to the centroid
    void sortVertices() {
        // Get centroid
        Point center = this->findCentroid();
        std::vector<Point> sortedPoints = vertices;
        // Sort based on angle relative to the centroid
        std::sort(sortedPoints.begin(), sortedPoints.end(), [&](const Point& a, const Point& b) {
            return angle(a, center) < angle(b, center);
            });
        vertices = sortedPoints;
    }
    bool isInside(Point p)
    {
        size_t size = this->size();
        int counter = 0;
        int i;
        double xinters;
        Point p1, p2;

        p1 = vertices[0];
        for (i = 1; i <= size; i++) {
            p2 = vertices[i % size];
            if (p.y > std::min(p1.y, p2.y)) {
                if (p.y <= std::max(p1.y, p2.y)) {
                    if (p.x <= std::max(p1.x, p2.x)) {
                        if (p1.y != p2.y) {
                            xinters = (p.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x;
                            if (p1.x == p2.x || p.x <= xinters)
                                counter++;
                        }
                    }
                }
            }
            
            p1 = p2;
        }

        if (counter % 2 == 0)
            return false;
        else
            return true;
    }
};


Polygon polygonIntersection(const Polygon& polygon1, const Polygon& polygon2) {
    Polygon result = Polygon();

    // Iterate through all edges of polygon1
    for (size_t i = 0; i < polygon1.size(); ++i) {
        size_t next = (i + 1) % polygon1.size();
        const Point& p1 = polygon1.vertices[i];
        const Point& p2 = polygon1.vertices[next];

        // Iterate through all edges of polygon2
        for (size_t j = 0; j < polygon2.size(); ++j) {
            size_t next2 = (j + 1) % polygon2.size();
            const Point& p3 = polygon2.vertices[j];
            const Point& p4 = polygon2.vertices[next2];

            // Check if the edges intersect
            Point intersection(0, 0);
            if (intersect(p1, p2, p3, p4, intersection)) {

                if (!result.contains(intersection)) {
                    result.add(intersection);
                }
            }
        }
    }
    return result;
}


void Calculate(Polygon InA , Polygon InB , Polygon &DiffA , Polygon &DiffB , Polygon &Union , Polygon &Intersection) {
    
    InA.sortVertices();
    InA.print("A");

    InB.sortVertices();
    InB.print("B");
    
    Polygon intersectionPoints = polygonIntersection(InA, InB);


    for (Point p : InA.vertices) {
        if (InB.isInside(p)) {
            Intersection.add(p);
            DiffB.add(p);
        }
        else {
            DiffA.add(p);
            Union.add(p);
        }
    }

    for (Point p : InB.vertices) {
        if (InA.isInside(p)) {
            Intersection.add(p);
            DiffA.add(p);
        }
        else {
            Union.add(p);
            DiffB.add(p);
        }
    }

    for (Point p : intersectionPoints.vertices) {
        Intersection.add(p);
        DiffA.add(p);
        DiffB.add(p);
        Union.add(p);
    }

    DiffA.sortVertices();
    DiffA.print("Difference A from B");
    DiffB.sortVertices();
    DiffB.print("Difference B from A");
    Union.sortVertices();
    Union.print("Union of A and B");
    Intersection.sortVertices();
    Union.print("Intersection of A and B");

}



//#include <opencv2/core.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <iostream>
//
//void convertToArayOfArray(Polygon input , vector<vector<cv::Point>> &contour) {
//    vector<cv::Point> temp;
//    for (Point p : input.vertices) {
//        temp.push_back(cv::Point(p.x, p.y));
//    }
//    contour.push_back(temp);
//
//}

bool isStringDigit(string input) {
    for (char a : input) {
        if (!std::isdigit(a))return false;
    }
    return true;
}

vector<string> splitString(string InputString, string delimeter) {
    size_t delimeterLenght = delimeter.size();
    size_t start = 0;
    size_t end = delimeterLenght;

    vector<string> result{};


    string temp;
    while ((end = InputString.find(delimeter , start)) != std::string::npos) {
        temp = InputString.substr(start, end-start);
        start = end + delimeterLenght;
        result.push_back(temp);
    }
    temp = InputString.substr(start, end - start);
    result.push_back(temp);
    return result;
}

int main(int argc , char* argv[]) {

    if (argc != 2) {
        cout << "Wrong argument format. \n ./Polygon.exe filename.txt\n";
        return -1;
    }

    Polygon a{};
    Polygon b{};

    std::fstream inputfile;
    inputfile.open(argv[1], std::ios::in);
    bool isA = true;
    double xPoint = 0;
    double yPoint = 0;

    if (inputfile.is_open()) {
        string line;
        while (std::getline(inputfile, line)) {
            if (line == "/////")isA = false;
            vector<string> temp = splitString(line, ",");
            
            if (temp.size() == 2) {
                if (isStringDigit(temp[0]))
                    xPoint = std::stod(temp[0]);
                else {
                    cout << "Points provided in file contain non digits\n";
                    return -1;
                }
                if (isStringDigit(temp[1]))
                    yPoint = std::stod(temp[1]);
                else {
                    cout << "Points provided in file contain non digits\n";
                    return -1;
                }

                cout << xPoint << " , " << yPoint << "\n";


                if (isA) {
                    a.add(Point(xPoint, yPoint));
                }
                else {
                    b.add(Point(xPoint, yPoint));
                }


            }
        }
    }
    else {
        cout << "Unable to find input file. \n";
        return -1;
    }



    Polygon Adifference, Bdifference, _union, intersection;


    Calculate(a, b, Adifference, Bdifference ,_union, intersection);

    return 0;
}