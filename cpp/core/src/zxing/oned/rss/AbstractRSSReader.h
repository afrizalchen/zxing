//
//  AbstractRSSReader.h
//  zxing-cpp
//
//  Created by Afrizal Chen on 3/8/13.
//
//

#ifndef __zxing_cpp__AbstractRSSReader__
#define __zxing_cpp__AbstractRSSReader__

#include <zxing/oned/OneDReader.h>

namespace zxing {
    namespace oned {
        namespace rss {
            class AbstractRSSReader : public OneDReader{
            private:
                static const int MAX_AVG_VARIANCE;
                static const int MAX_INDIVIDUAL_VARIANCE;
                static const float MIN_FINDER_PATTERN_RATIO;
                static const float MAX_FINDER_PATTERN_RATIO;
                
                std::vector<int> decodeFinderCounters;
                std::vector<int> dataCharacterCounters;
                std::vector<float> oddRoundingErrors;
                std::vector<float> evenRoundingErrors;
                std::vector<int> oddCounts;
                std::vector<int> evenCounts;
            public:
                std::vector<int> &getDecodeFinderCounters();
                std::vector<int> &getDataCharacterCounters();
                std::vector<float> &getOddRoundingErrors();
                std::vector<float> &getEvenRoundingErrors();
                std::vector<int> &getOddCounts();
                std::vector<int> &getEvenCounts();
                virtual ~AbstractRSSReader();
            protected:
                AbstractRSSReader();
                static int parseFinderValue(std::vector<int> &counters, std::vector<int const*> const& finderPatterns);
                static int count(std::vector<int> &array);
                static void increment(std::vector<int> &array, std::vector<float> &errors);
                static void decrement(std::vector<int> &array, std::vector<float> &errors);
                static bool isFinderPattern(std::vector<int> &counters);
            };
            
        }
    }
}


#endif /* defined(__zxing_cpp__AbstractRSSReader__) */
