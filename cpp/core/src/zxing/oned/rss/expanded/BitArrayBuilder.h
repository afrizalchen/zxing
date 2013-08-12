//
//  BitArrayBuilder.h
//  zxing-cpp
//
//  Created by Afrizal Chen on 3/8/13.
//
//

#ifndef __zxing_cpp__BitArrayBuilder__
#define __zxing_cpp__BitArrayBuilder__

#import <zxing/common/BitArray.h>
#import <zxing/oned/rss/expanded/ExpandedPair.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                class BitArrayBuilder {
                private:
                    BitArrayBuilder();
                public:
                    static Ref<BitArray> buildBitArray(std::vector< Ref<ExpandedPair> > &pairs);
                };
            }
        }
    }
}

#endif /* defined(__zxing_cpp__BitArrayBuilder__) */
