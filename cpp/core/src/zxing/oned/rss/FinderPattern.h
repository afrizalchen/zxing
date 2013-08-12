//
//  FinderPattern.h
//  zxing-cpp
//
//  Created by Afrizal Chen on 2/8/13.
//
//

#ifndef __zxing_cpp__FinderPattern__
#define __zxing_cpp__FinderPattern__

#include <zxing/ResultPoint.h>
#include <zxing/common/Array.h>
#include <zxing/common/Counted.h>

namespace zxing {
    namespace oned {
        namespace rss {
            class FinderPattern : public Counted {
            private:
                int _value;
                std::vector<int> _startEnd;
                ArrayRef< Ref<ResultPoint> > _resultPoints;
            public:
                FinderPattern(int value, std::vector<int> &startEnd, int start, int end, int rowNumber);
                ~FinderPattern();
                int getValue() const;
                std::vector<int> &getStartEnd();
                ArrayRef< Ref<ResultPoint> > getResultPoints();
                bool operator==(const FinderPattern &other) const;
                bool equals(const FinderPattern &other) const;
                bool equals(Ref<FinderPattern> other) const;
            };
            
        }
    }
}

#endif /* defined(__zxing_cpp__FinderPattern__) */
