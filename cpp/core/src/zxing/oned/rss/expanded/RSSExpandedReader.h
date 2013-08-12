//
//  RSSExpandedReader.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 4/8/13.
//
//

#ifndef __ZXingWidget__RSSExpandedReader__
#define __ZXingWidget__RSSExpandedReader__


#include <zxing/oned/rss/AbstractRSSReader.h>
#include <zxing/oned/rss/expanded/ExpandedPair.h>
#include <zxing/oned/rss/expanded/ExpandedRow.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                class RSSExpandedReader : public AbstractRSSReader {
                private:
                    std::vector< Ref<ExpandedPair> > _pairs;
                    std::vector< Ref<ExpandedRow> > _rows;
                    std::vector<int> _startEnd;
                    bool _startFromEven;
                    static const std::vector<int const*> FINDER_PATTERNS;
                public:
                    RSSExpandedReader();
                    Ref<Result> decodeRow(int rowNumber, Ref<BitArray> row);
                    void reset();
                private:
                    std::vector< Ref<ExpandedPair> > decodeRow2pairs(int rowNumber, Ref<BitArray> row);
                    std::vector< Ref<ExpandedPair> > checkRows(bool reverse);
                    std::vector< Ref<ExpandedPair> > checkRows(std::vector< Ref<ExpandedRow> > &collectedRows, int currentRow);
                    static bool isValidSequence(std::vector< Ref<ExpandedPair> > &pairs);
                    void storeRow(int rowNumber, bool wasReversed);
                    static void removePartialRows(std::vector< Ref<ExpandedPair> > &pairs, std::vector< Ref<ExpandedRow> > &rows);
                    static bool isPartialRow(std::vector< Ref<ExpandedPair> > &pairs, std::vector< Ref<ExpandedRow> > &rows);
                    static Ref<Result> constructResult(std::vector< Ref<ExpandedPair> > &pairs);
                    bool checkChecksum();
                    static int getNextSecondBar(Ref<BitArray> row, int initialPos);
                    Ref<ExpandedPair> retrieveNextPair(Ref<BitArray> row, std::vector< Ref<ExpandedPair> > &previousPairs, int rowNumber);
                    void findNextPair(Ref<BitArray> row, std::vector< Ref<ExpandedPair> > &previousPairs, int forcedOffset);
                    static void reverseCounters(std::vector<int> &counters);
                    Ref<FinderPattern> parseFoundFinderPattern(Ref<BitArray> row, int rowNumber, bool oddPattern);
                    Ref<DataCharacter> decodeDataCharacter(Ref<BitArray> row, Ref<FinderPattern> pattern, bool isOddPattern, bool leftChar);
                    static bool isNotA1left(Ref<FinderPattern> pattern, bool isOddPattern, bool leftChar);
                    void adjustOddEvenCounts(int numModules);
                    };
            }
        }
    }
}

#endif /* defined(__ZXingWidget__RSSExpandedReader__) */
