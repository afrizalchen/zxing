//
//  RSS14Reader.h
//  zxing-cpp
//
//  Created by Afrizal Chen on 3/8/13.
//
//

#ifndef __zxing_cpp__RSS14Reader__
#define __zxing_cpp__RSS14Reader__

#include <zxing/DecodeHints.h>
#include <zxing/Result.h>
#include <zxing/oned/rss/AbstractRSSReader.h>
#include <zxing/oned/rss/DataCharacter.h>
#include <zxing/oned/rss/Pair.h>

namespace zxing {
    namespace oned {
        namespace rss {
            class RSS14Reader : public AbstractRSSReader {
            private:
                std::vector< Ref<Pair> > possibleLeftPairs;
                std::vector< Ref<Pair> > possibleRightPairs;
                static const std::vector<int const*> FINDER_PATTERNS;
            public:
                RSS14Reader();
                virtual ~RSS14Reader();
                Ref<Result> decodeRow(int rowNumber, Ref<BitArray> row);
                void reset();
            private:
                static void addOrTally(std::vector< Ref<Pair> > &possiblePairs, Ref<Pair> pair);
                static Ref<Result> constructResult(Ref<Pair> leftPair, Ref<Pair> rightPair);
                static bool checkChecksum(Ref<Pair> leftPair, Ref<Pair> rightPair);
                Ref<Pair> decodePair(Ref<BitArray> row, bool right, int rowNumber, DecodeHints hints);
                Ref<DataCharacter> decodeDataCharacter(Ref<BitArray> row, Ref<FinderPattern> pattern, bool outsideChar);
                std::vector<int> findFinderPattern(Ref<BitArray> row, int rowOffset, bool rightFinderPattern);
                Ref<FinderPattern> parseFoundFinderPattern(Ref<BitArray> row, int rowNumber, bool right, std::vector<int> &startEnd);
                void adjustOddEvenCounts(bool outsideChar, int numModules);
            };
            
        }
    }
}

#endif /* defined(__zxing_cpp__RSS14Reader__) */
