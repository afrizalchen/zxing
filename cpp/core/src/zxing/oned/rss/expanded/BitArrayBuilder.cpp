//
//  BitArrayBuilder.cpp
//  zxing-cpp
//
//  Created by Afrizal Chen on 3/8/13.
//
//


#include <zxing/oned/rss/expanded/BitArrayBuilder.h>

using zxing::BitArray;
using zxing::Ref;
using zxing::oned::rss::expanded::BitArrayBuilder;
using zxing::oned::rss::expanded::ExpandedPair;

BitArrayBuilder::BitArrayBuilder() {
    
}

Ref<BitArray> BitArrayBuilder::buildBitArray(std::vector< Ref<ExpandedPair> > &pairs) {
    int charNumber = (pairs.size() << 1) - 1;
    if (pairs[pairs.size() - 1]->getRightChar().empty()) {
        charNumber -= 1;
    }
    
    int size = 12 * charNumber;
    
    Ref<BitArray> binary(new BitArray(size));
    int accPos = 0;
    
    Ref<ExpandedPair> firstPair = pairs[0];
    int firstValue = firstPair->getRightChar()->getValue();
    for(int i = 11; i >= 0; --i){
        if ((firstValue & (1 << i)) != 0) {
            binary->set(accPos);
        }
        accPos++;
    }
    
    int pairsSize = pairs.size();
    for(int i = 1; i < pairsSize; ++i){
        Ref<ExpandedPair> currentPair = pairs[i];
        
        int leftValue = currentPair->getLeftChar()->getValue();
        for(int j = 11; j >= 0; --j){
            if ((leftValue & (1 << j)) != 0) {
                binary->set(accPos);
            }
            accPos++;
        }
        
        if(currentPair->getRightChar().empty() == false){
            int rightValue = currentPair->getRightChar()->getValue();
            for(int j = 11; j >= 0; --j){
                if ((rightValue & (1 << j)) != 0) {
                    binary->set(accPos);
                }
                accPos++;
            }
        }
    }
    return binary;
}
