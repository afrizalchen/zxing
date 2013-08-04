//
//  DataCharacter.h
//  zxing-cpp
//
//  Created by Afrizal Chen on 2/8/13.
//
//

#ifndef __zxing_cpp__DataCharacter__
#define __zxing_cpp__DataCharacter__

#include <zxing/common/Counted.h>

namespace zxing {
    namespace oned {
        namespace rss {
            class DataCharacter : public Counted {
            private:
                int _value;
                int _checksumPortion;
            public:
                DataCharacter(int value, int checksumPortion);
                ~DataCharacter();
                int getValue() const;
                int getChecksumPortion() const;
                bool operator==(const DataCharacter &other) const;
            };
        }
    }
}

#endif /* defined(__zxing_cpp__DataCharacter__) */
