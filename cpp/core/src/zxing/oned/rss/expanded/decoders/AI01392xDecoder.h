//
//  AI01392xDecoder.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 9/8/13.
//
//

#ifndef __ZXingWidget__AI01392xDecoder__
#define __ZXingWidget__AI01392xDecoder__

#include <zxing/oned/rss/expanded/decoders/AI01decoder.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class AI01392xDecoder : public AI01decoder {
                    private:
                    public:
                        AI01392xDecoder(Ref<BitArray> information);
                        virtual ~AI01392xDecoder();
                    private:
                        Ref<String> parseInformation();
                    };
                }
            }
        }
    }
}


#endif /* defined(__ZXingWidget__AI01392xDecoder__) */
