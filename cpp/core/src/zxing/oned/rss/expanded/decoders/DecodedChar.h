//
//  DecodedChar.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 5/8/13.
//
//

#ifndef __ZXingWidget__DecodedChar__
#define __ZXingWidget__DecodedChar__

#include <zxing/oned/rss/expanded/decoders/DecodedObject.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class DecodedChar : public DecodedObject {
                    private:
                        char _value;
                    public:
                        static const char FNC1;
                    public:
                        DecodedChar(int newPosition, char value);
                        char getValue();
                        bool isFNC1();
                    };
                }
            }
        }
    }
}

#endif /* defined(__ZXingWidget__DecodedChar__) */
