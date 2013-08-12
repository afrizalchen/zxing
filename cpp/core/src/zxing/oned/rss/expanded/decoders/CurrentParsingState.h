//
//  CurrentParsingState.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 4/8/13.
//
//

#ifndef __ZXingWidget__CurrentParsingState__
#define __ZXingWidget__CurrentParsingState__

#include <zxing/common/Counted.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class CurrentParsingState : public Counted {
                    private:
                        enum State {
                            NUMERIC,
                            ALPHA,
                            ISO_IEC_646
                        };
                        int _position;
                        State _encoding;
                    public:
                        CurrentParsingState();
                        int getPosition();
                        void setPosition(int position);
                        void incrementPosition(int delta);
                        bool isAlpha();
                        bool isNumeric();
                        bool isIsoIec646();
                        void setNumeric();
                        void setAlpha();
                        void setIsoIec646();
                    };
                }
            }
        }
    }
}

#endif /* defined(__ZXingWidget__CurrentParsingState__) */
