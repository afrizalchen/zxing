//
//  AI013x0xDecoder.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 9/8/13.
//
//

#ifndef __ZXingWidget__AI013x0xDecoder__
#define __ZXingWidget__AI013x0xDecoder__

#include <zxing/oned/rss/expanded/decoders/AI01weightDecoder.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class AI013x0xDecoder : public AI01weightDecoder {
                    private:
                    public:
                        AI013x0xDecoder(Ref<BitArray> information);
                        virtual ~AI013x0xDecoder();
                        Ref<String> parseInformation();
                    };
                }
            }
        }
    }
}

#endif /* defined(__ZXingWidget__AI013x0xDecoder__) */
