//
//  Pair.h
//  zxing-cpp
//
//  Created by Afrizal Chen on 2/8/13.
//
//

#ifndef __zxing_cpp__Pair__
#define __zxing_cpp__Pair__

#include <zxing/common/Counted.h>
#include <zxing/oned/rss/DataCharacter.h>
#include <zxing/oned/rss/FinderPattern.h>

namespace zxing {
    namespace oned {
        namespace rss {
            class Pair : public DataCharacter {
            private:
                Ref<FinderPattern> _finderPattern;
                int _count;
            public:
                Pair(int value, int checksumPortion, Ref<FinderPattern> finderPattern);
                ~Pair();
                Ref<FinderPattern> getFinderPattern() const;
                int getCount() const;
                void incrementCount();
            };
            
        }
    }
}
#endif /* defined(__zxing_cpp__Pair__) */
