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
                class ExpandedPair  : public Counted {
                private:
                    bool _mayBeLast;
                    Ref<DataCharacter> _leftChar;
                    Ref<DataCharacter> _rightChar;
                    Ref<FinderPattern> _finderPattern;
                public:
                    ExpandedPair(Ref<DataCharacter> leftChar, Ref<DataCharacter> rightChar, Ref<FinderPattern> finderPattern, bool mayBeLast);
                    Ref<DataCharacter> getLeftChar() const;
                    Ref<DataCharacter> getRightChar() const;
                    Ref<FinderPattern> getFinderPattern() const;
                    bool mayBeLast();
                    bool mustBeLast() const;
                    bool operator==(const ExpandedPair &other) const;
                    bool equals(const ExpandedPair &other) const;
                    bool equals(Ref<ExpandedPair> other) const;
                };
            }
        }
    }
}

#endif /* defined(__zxing_cpp__ExpandedPair__) */
