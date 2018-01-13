//
// Created by damir on 10-Jan-18.
//

#include "../hdr/FreeNode.h"


int FreeNode::id = 0;

FreeNode::FreeNode()
        : _id(id++) {}

FreeNode::FreeNode(char *block, size_t size)
        : _blockSize(size), _block(block), _id(id++), next(nullptr), prev(nullptr) {}

size_t FreeNode::getBlockSize() const {
    return _blockSize;
}

FreeNode::~FreeNode() {}

char *FreeNode::getBlock() {
    return _block;
}


std::ostream &operator<<(std::ostream &os, FreeNode &node) {
    char *blk = node.getBlock();

    os << "[ID: " << node._id << ", Size: " << node.getBlockSize() << ", Memory At:";// << addr << "]";

    char str[16];

    sprintf(str, "%p]", (void*)blk);

    os << str;

    return os;
}

void FreeNode::setBlock(char *block) {
    _block = block;
}

void FreeNode::setSize(size_t size) {
    _blockSize = size;
}


