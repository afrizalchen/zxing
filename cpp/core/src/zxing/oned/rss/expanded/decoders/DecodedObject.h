//
//  DecodedObject.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 4/8/13.
//
//

#ifndef __ZXingWidget__DecodedObject__
#define __ZXingWidget__DecodedObject__

#include <zxing/common/Counted.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class DecodedObject : public Counted {
                    private:
                        int _newPosition;
                    protected:
                        DecodedObject(int newPosition);
                    public:
                        int getNewPosition();
                    };
                }
            }
        }
    }
}

#endif /* defined(__ZXingWidget__DecodedObject__) */
