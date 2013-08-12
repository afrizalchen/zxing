//
//  AI01393xDecoder.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 9/8/13.
//
//

#ifndef __ZXingWidget__AI01393xDecoder__
#define __ZXingWidget__AI01393xDecoder__

#include <zxing/oned/rss/expanded/decoders/AI01decoder.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class AI01393xDecoder : public AI01decoder {
                    private:
                    public:
                        AI01393xDecoder(Ref<BitArray> information);
                        virtual ~AI01393xDecoder();
                    private:
                        Ref<String> parseInformation();
                    };
                }
            }
        }
    }
}


#endif /* defined(__ZXingWidget__AI01393xDecoder__) */
