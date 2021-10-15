//
// Created by sorok on 15.10.2021.
//

#ifndef Buildings_QuadTreeNode_h
#define Buildings_QuadTreeNode_h

class QTND;
class QTN;

typedef void(QTNDBlock)(QTND data);
typedef void(QTNTraverseBlock)(QTN *node);


typedef struct QTND {
    double latitude;
    double longitude;
    void *data;
} QTND;

QTND QTNDMake(double latitude, double longitude, void *data);


typedef struct BoundingBox {
    double x0; double y0;
    double x1; double y1;
} BoundingBox;

BoundingBox BoundingBoxMake(double x0, double y0, double x1, double y1);

bool BoundingBoxContainsData(BoundingBox boundary, QTND data);
bool BoundingBoxIntersectsBoundingBox(BoundingBox boundary, BoundingBox test);


class QTN {
protected:
    QTN *northWest;
    QTN *northEast;
    QTN *southWest;
    QTN *southEast;
    QTND *place;
    BoundingBox boundary;
    int capacity;
    int count;
public:
    QTN(BoundingBox boundary, int capacity);
    ~QTN();

    static QTN* QTNMake(BoundingBox boundary, int capacity);

    void gatherDataWithinBoundary(BoundingBox boundary, QTNDBlock block);
    void traverse(QTNTraverseBlock block);
    bool insert(QTND data);
    void subdivide();

    QTND* getPlace() const;
    BoundingBox getBoundary() const;
    int getCapacity() const;
};

class QuadTree : public QTN {
public:
    QuadTree(QTND *data, BoundingBox bourdary, int capacity);
    static QuadTree* QuadTreeMake(QTND *data, BoundingBox boundary, int capacity);
};

#endif