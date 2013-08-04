//
//  ExpandedPair.h
//  zxing-cpp
//
//  Created by Afrizal Chen on 3/8/13.
//
//

#ifndef __zxing_cpp__ExpandedPair__
#define __zxing_cpp__ExpandedPair__

#include <zxing/oned/rss/DataCharacter.h>
#include <zxing/oned/rss/FinderPattern.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                class ExpandedPair {
                private:
                    bool _mayBeLast;
                    Ref<DataCharacter> _leftChar;
                    Ref<DataCharacter> _rightChar;
                    Ref<FinderPattern> _finderPattern;
                 private:   
                    ExpandedPair(Ref<DataCharacter> leftChar, Ref<DataCharacter> rightChar, Ref<FinderPattern> finderPattern, bool mayBeLast);
                    bool mayBeLast();
                    Ref<DataCharacter> getLeftChar();
                    Ref<DataCharacter> getRightChar();
                    Ref<FinderPattern> getFinderPattern();
                public:
                    bool mustBeLast();
                    bool operator==(const ExpandedPair &other) const;
                };
            }
        }
    }
}

#endif /* defined(__zxing_cpp__ExpandedPair__) */
