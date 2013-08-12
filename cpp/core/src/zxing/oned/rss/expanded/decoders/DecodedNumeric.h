//
//  DecodedNumeric.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 5/8/13.
//
//

#ifndef __ZXingWidget__DecodedNumeric__
#define __ZXingWidget__DecodedNumeric__

#include <zxing/oned/rss/expanded/decoders/DecodedObject.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class DecodedNumeric : public DecodedObject {
                    public:
                        static const int FNC1;
                    private:
                        int _firstDigit;
                        int _secondDigit;
                    public:
                        DecodedNumeric(int newPosition, int firstDigit, int secondDigit);
                        int getFirstDigit();
                        int getSecondDigit();
                        int getValue();
                        bool isFirstDigitFNC1();
                        bool isSecondDigitFNC1();
                        bool isAnyFNC1();
                    };
                }
            }
        }
    }
}

#endif /* defined(__ZXingWidget__DecodedNumeric__) */
