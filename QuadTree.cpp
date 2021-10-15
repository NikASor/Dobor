#include <iostream>
#include "QuadTree.h"

QTND QTNDMake(double latitude, double longitude, void *data) {

    QTND nodeData;
    nodeData.latitude = latitude;
    nodeData.longitude = longitude;
    nodeData.data = data;

    return nodeData;

}


BoundingBox BoundingBoxMake(double x0, double y0, double x1, double y1) {

    BoundingBox boundingBox;
    boundingBox.x0 = x0;
    boundingBox.y0 = y0;
    boundingBox.x1 = x1;
    boundingBox.y1 = y1;

    return boundingBox;

}


bool BoundingBoxContainsData(BoundingBox boundary, QTND data) {

    bool containsX = boundary.x0 <= data.latitude && data.latitude <= boundary.x1;
    bool containsY = boundary.y0 <= data.longitude && data.longitude <= boundary.y1;

    return containsX && containsY;

}


bool BoundingBoxIntersectsBoundingBox(BoundingBox boundary, BoundingBox test) {

    return (boundary.x0 <= test.x1 && boundary.x1 >= test.x0 && boundary.y0 <= test.y1 && boundary.y1 >= test.y0);

}


QTN::QTN(BoundingBox boundary, int capacity) {

    this->northWest = NULL;
    this->northEast = NULL;
    this->southWest = NULL;
    this->southEast = NULL;

    this->boundary = boundary;
    this->capacity = capacity;
    this->count = 0;
    this->place = (QTND *)malloc ( sizeof ( QTND ) * capacity );

}


QTN::~QTN() {

    // Free all child nodes.
    if ( this->northWest != NULL ) delete this->northWest;
    if ( this->northEast != NULL ) delete this->northEast;
    if ( this->southWest != NULL ) delete this->southWest;
    if ( this->southEast != NULL ) delete this->southEast;

    // Free all nodes contained within this node.
    for ( int i = 0; i < this->count; i++ ) {
        free ( this->place[i].data );
    }

    // Free this node.
    delete this->place;

}


QTN* QTN::QTNMake(BoundingBox boundary, int capacity) {

    QTN *node = new QTN ( boundary, capacity );
    return node;

}


void QTN::gatherDataWithinBoundary(BoundingBox boundary, QTNDBlock block) {

    // If range is not contained in the node's boundingBox then bail
    if ( !BoundingBoxIntersectsBoundingBox ( this->boundary, boundary ) ) {
        return;
    }

    for ( int i = 0; i < this->count; i++ ) {
        // Gather place contained in range
        if ( BoundingBoxContainsData ( boundary, this->place[i] ) ) {
            block ( this->place[i] );
        }
    }

    // Bail if node is leaf
    if ( this->northWest == NULL ) {
        return;
    }

    // Otherwise traverse down the tree
    this->northWest->gatherDataWithinBoundary ( boundary, block );
    this->northEast->gatherDataWithinBoundary ( boundary, block );
    this->southWest->gatherDataWithinBoundary ( boundary, block );
    this->southEast->gatherDataWithinBoundary ( boundary, block );

}


void QTN::traverse(QTNTraverseBlock block) {

    block ( this );

    if ( this->northWest == NULL ) {
        return;
    }

    this->northWest->traverse ( block );
    this->northEast->traverse ( block );
    this->southWest->traverse ( block );
    this->southEast->traverse ( block );

}


bool QTN::insert(QTND data) {

    // Return if our coordinate is not in the boundingBox
    if ( !BoundingBoxContainsData ( this->boundary, data ) ) {
        return false;
    }

    // Add the coordinate to the place array.
    if ( this->count < this->capacity ) {
        this->place[this->count++] = data;
        return true;
    }

    // Check to see if the current node is a leaf, if it is, split.
    if ( this->northWest == NULL ) {
        this->subdivide();
    }

    // Traverse the tree
    if ( this->northWest->insert ( data ) ) return true;
    if ( this->northEast->insert ( data ) ) return true;
    if ( this->southWest->insert ( data ) ) return true;
    if ( this->southEast->insert ( data ) ) return true;

    // Default. Was unable to add the node.
    return false;

}


void QTN::subdivide() {

    BoundingBox box = this->boundary;

    // Spit the quadrant into four equal parts.
    double xMid = (box.x1 + box.x0) / 2.0;
    double yMid = (box.y1 + box.y0) / 2.0;

    // Create the north west bounding box.
    BoundingBox northWest = BoundingBoxMake ( box.x0, box.y0, xMid, yMid );
    this->northWest = new QTN ( northWest, this->capacity );

    // Create the north east bounding box.
    BoundingBox northEast = BoundingBoxMake ( xMid, box.y0, box.x1, yMid );
    this->northEast = new QTN ( northEast, this->capacity );

    // Create the south west bounding box.
    BoundingBox southWest = BoundingBoxMake ( box.x0, yMid, xMid, box.y1 );
    this->southWest = new QTN ( southWest, this->capacity );

    // Create the south east bounding box.
    BoundingBox southEast = BoundingBoxMake ( xMid, yMid, box.x1, box.y1 );
    this->southEast = new QTN ( southEast, this->capacity );

}


QTND* QTN::getPlace() const {

    return this->place;

}


BoundingBox QTN::getBoundary() const {

    return this->boundary;

}


int QTN::getCapacity() const {

    return this->capacity;

}

QuadTree::QuadTree(QTND *data, BoundingBox bourdary, int capacity) : QTN ( boundary, capacity ) {

    for ( int i = 0; i < sizeof ( data ) / sizeof ( QTND ); i++ ) {
        this->insert ( data[i] );
    }

}


QTN* QuadTreeMake(QTND *data, BoundingBox boundary, int capacity) {

    QuadTree *tree = new QuadTree ( data, boundary, capacity );
    return tree;

}