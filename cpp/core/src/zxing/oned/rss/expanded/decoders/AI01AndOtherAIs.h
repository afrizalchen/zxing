//
//  AI01AndOtherAIs.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 9/8/13.
//
//

#ifndef __ZXingWidget__AI01AndOtherAIs__
#define __ZXingWidget__AI01AndOtherAIs__

#include <zxing/oned/rss/expanded/decoders/AI01decoder.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class AI01AndOtherAIs : public AI01decoder {
                    private:
                    public:
                        AI01AndOtherAIs(Ref<BitArray> information);
                        virtual ~AI01AndOtherAIs();
                    private:
                        Ref<String> parseInformation();
                    };
                }
            }
        }
    }
}

#endif /* defined(__ZXingWidget__AI01AndOtherAIs__) */
