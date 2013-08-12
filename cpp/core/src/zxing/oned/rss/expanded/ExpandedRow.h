//
//  ExpandedRow.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 4/8/13.
//
//

#ifndef __ZXingWidget__ExpandedRow__
#define __ZXingWidget__ExpandedRow__

#include <zxing/oned/rss/expanded/ExpandedPair.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                class ExpandedRow  : public Counted {
                private:
                    std::vector< Ref<ExpandedPair> > _pairs;
                    int _rowNumber;
                    bool _wasReversed;
                public:
                    ExpandedRow(std::vector< Ref<ExpandedPair> > &pairs, int rowNumber, bool wasReversed);
                    std::vector< Ref<ExpandedPair> > &getPairs();
                    int getRowNumber();
                    bool isReversed();
                    bool isEquivalent(std::vector< Ref<ExpandedPair> > &otherPairs);
                    bool operator==(const ExpandedRow &other) const;
                };
            }
        }
    }
}

#endif /* defined(__ZXingWidget__ExpandedRow__) */
