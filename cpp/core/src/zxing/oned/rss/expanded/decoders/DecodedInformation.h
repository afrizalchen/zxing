//
//  DecodedInformation.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 5/8/13.
//
//

#ifndef __ZXingWidget__DecodedInformation__
#define __ZXingWidget__DecodedInformation__

#include <zxing/oned/rss/expanded/decoders/DecodedObject.h>
#include <zxing/common/Str.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class DecodedInformation : public DecodedObject {
                    private:
                        Ref<String> _newString;
                        int _remainingValue;
                        bool _remaining;
                    public:
                        DecodedInformation(int newPosition, Ref<String> newString);
                        DecodedInformation(int newPosition, Ref<String> newString, int remainingValue);
                        Ref<String> getNewString();
                        bool isRemaining();
                        int getRemainingValue();
                    };
                }
            }
        }
    }
}

#endif /* defined(__ZXingWidget__DecodedInformation__) */
