//
//  RSSExpandedReader.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 4/8/13.
//
//

#include <algorithm>
#include <cmath>
#include <zxing/NotFoundException.h>
#include <zxing/oned/rss/RSSUtils.h>
#include <zxing/oned/rss/expanded/BitArrayBuilder.h>
#include <zxing/oned/rss/expanded/RSSExpandedReader.h>
#include <zxing/oned/rss/expanded/decoders/AbstractExpandedDecoder.h>

using zxing::Ref;
using zxing::Result;
using zxing::BitArray;
using zxing::oned::rss::FinderPattern;
using zxing::oned::rss::DataCharacter;
using zxing::oned::rss::expanded::BitArrayBuilder;
using zxing::oned::rss::expanded::ExpandedPair;
using zxing::oned::rss::expanded::ExpandedRow;
using zxing::oned::rss::expanded::RSSExpandedReader;
using zxing::oned::rss::expanded::decoders::AbstractExpandedDecoder;

namespace {
    const int SYMBOL_WIDEST[5] = {7, 5, 4, 3, 1};
    const int EVEN_TOTAL_SUBSET[5] = {4, 20, 52, 104, 204};
    const int GSUM[5] = {0, 348, 1388, 2948, 3988};
    
    const int FINDER_PATTERNS_[6][4] = {
        {1,8,4,1}, // A
        {3,6,4,1}, // B
        {3,4,6,1}, // C
        {3,2,8,1}, // D
        {2,6,5,1}, // E
        {2,2,9,1}  // F
    };
    
    const int WEIGHTS[23][8] = {
        {  1,   3,   9,  27,  81,  32,  96,  77}, // 1
        { 20,  60, 180, 118, 143,   7,  21,  63}, // 2
        {189, 145,  13,  39, 117, 140, 209, 205}, // 3
        {193, 157,  49, 147,  19,  57, 171,  91}, // 4
        { 62, 186, 136, 197, 169,  85,  44, 132}, // 5
        {185, 133, 188, 142,   4,  12,  36, 108}, // 6
        {113, 128, 173,  97,  80,  29,  87,  50}, // 7
        {150,  28,  84,  41, 123, 158,  52, 156}, // 8
        { 46, 138, 203, 187, 139, 206, 196, 166}, // 9
        { 76,  17,  51, 153,  37, 111, 122, 155}, // 10
        { 43, 129, 176, 106, 107, 110, 119, 146}, // 11
        { 16,  48, 144,  10,  30,  90,  59, 177}, // 12
        {109, 116, 137, 200, 178, 112, 125, 164}, // 13
        { 70, 210, 208, 202, 184, 130, 179, 115}, // 14
        {134, 191, 151,  31,  93,  68, 204, 190}, // 15
        {148,  22,  66, 198, 172,   94, 71,   2}, // 16
        {  6,  18,  54, 162,  64,  192,154,  40}, // 17
        {120, 149,  25,  75,  14,   42,126, 167}, // 18
        { 79,  26,  78,  23,  69,  207,199, 175}, // 19
        {103,  98,  83,  38, 114, 131, 182, 124}, // 20
        {161,  61, 183, 127, 170,  88,  53, 159}, // 21
        { 55, 165,  73,   8,  24,  72,   5,  15}, // 22
        { 45, 135, 194, 160,  58, 174, 100,  89}  // 23
    };
    
    const int FINDER_PAT_A = 0;
    const int FINDER_PAT_B = 1;
    const int FINDER_PAT_C = 2;
    const int FINDER_PAT_D = 3;
    const int FINDER_PAT_E = 4;
    const int FINDER_PAT_F = 5;
    
    const int MAX_PAIRS = 11;
 
    #define VECTOR_INIT(v) v, v + sizeof(v)/sizeof(v[0])
    const int FINDER_PATTERN_SEQUENCES_1[2] = { FINDER_PAT_A, FINDER_PAT_A };
    const int FINDER_PATTERN_SEQUENCES_2[3] = { FINDER_PAT_A, FINDER_PAT_B, FINDER_PAT_B };
    const int FINDER_PATTERN_SEQUENCES_3[4] = { FINDER_PAT_A, FINDER_PAT_C, FINDER_PAT_B, FINDER_PAT_D };
    const int FINDER_PATTERN_SEQUENCES_4[5] = { FINDER_PAT_A, FINDER_PAT_E, FINDER_PAT_B, FINDER_PAT_D, FINDER_PAT_C };
    const int FINDER_PATTERN_SEQUENCES_5[6] = { FINDER_PAT_A, FINDER_PAT_E, FINDER_PAT_B, FINDER_PAT_D, FINDER_PAT_D, FINDER_PAT_F };
    const int FINDER_PATTERN_SEQUENCES_6[7] = { FINDER_PAT_A, FINDER_PAT_E, FINDER_PAT_B, FINDER_PAT_D, FINDER_PAT_E, FINDER_PAT_F, FINDER_PAT_F };
    const int FINDER_PATTERN_SEQUENCES_7[8] = { FINDER_PAT_A, FINDER_PAT_A, FINDER_PAT_B, FINDER_PAT_B, FINDER_PAT_C, FINDER_PAT_C, FINDER_PAT_D, FINDER_PAT_D };
    const int FINDER_PATTERN_SEQUENCES_8[9] = { FINDER_PAT_A, FINDER_PAT_A, FINDER_PAT_B, FINDER_PAT_B, FINDER_PAT_C, FINDER_PAT_C, FINDER_PAT_D, FINDER_PAT_E, FINDER_PAT_E };
    const int FINDER_PATTERN_SEQUENCES_9[10] = { FINDER_PAT_A, FINDER_PAT_A, FINDER_PAT_B, FINDER_PAT_B, FINDER_PAT_C, FINDER_PAT_C, FINDER_PAT_D, FINDER_PAT_E, FINDER_PAT_F, FINDER_PAT_F };
    const int FINDER_PATTERN_SEQUENCES_10[11] = { FINDER_PAT_A, FINDER_PAT_A, FINDER_PAT_B, FINDER_PAT_B, FINDER_PAT_C, FINDER_PAT_D, FINDER_PAT_D, FINDER_PAT_E, FINDER_PAT_E, FINDER_PAT_F, FINDER_PAT_F };
    const std::vector<int> FPS_1 = std::vector<int>(VECTOR_INIT(FINDER_PATTERN_SEQUENCES_1));
    const std::vector<int> FPS_2 = std::vector<int>(VECTOR_INIT(FINDER_PATTERN_SEQUENCES_2));
    const std::vector<int> FPS_3 = std::vector<int>(VECTOR_INIT(FINDER_PATTERN_SEQUENCES_3));
    const std::vector<int> FPS_4 = std::vector<int>(VECTOR_INIT(FINDER_PATTERN_SEQUENCES_4));
    const std::vector<int> FPS_5 = std::vector<int>(VECTOR_INIT(FINDER_PATTERN_SEQUENCES_5));
    const std::vector<int> FPS_6 = std::vector<int>(VECTOR_INIT(FINDER_PATTERN_SEQUENCES_6));
    const std::vector<int> FPS_7 = std::vector<int>(VECTOR_INIT(FINDER_PATTERN_SEQUENCES_7));
    const std::vector<int> FPS_8 = std::vector<int>(VECTOR_INIT(FINDER_PATTERN_SEQUENCES_8));
    const std::vector<int> FPS_9 = std::vector<int>(VECTOR_INIT(FINDER_PATTERN_SEQUENCES_9));
    const std::vector<int> FPS_10 = std::vector<int>(VECTOR_INIT(FINDER_PATTERN_SEQUENCES_10));
    const std::vector<int> ARR_FINDER_PATTERN_SEQUENCES[10] =
      { FPS_1, FPS_2, FPS_3, FPS_4, FPS_5, FPS_6, FPS_7, FPS_8, FPS_9, FPS_10 };
    const std::vector< std::vector<int> > FINDER_PATTERN_SEQUENCES = std::vector< std::vector<int> >(VECTOR_INIT(ARR_FINDER_PATTERN_SEQUENCES));
}

const std::vector<int const*>
RSSExpandedReader::FINDER_PATTERNS (VECTOR_INIT(FINDER_PATTERNS_));

RSSExpandedReader::RSSExpandedReader() {
    _pairs = std::vector< Ref<ExpandedPair> >(2);
    _rows = std::vector< Ref<ExpandedRow> >();
    _startEnd = std::vector<int>(2);
    _startFromEven = false;
}

Ref<Result> RSSExpandedReader::decodeRow(int rowNumber, Ref<BitArray> row) {
    int width = row->getSize();
    for(int i = 0; i < width; i++) {
        printf("%d ", row->get(i) ? 1 : 0);
        if ((i + 1) % 10 == 0) printf("- ");
        if ((i + 1) % 40 == 0) printf("\n");
    }
    //printf("row-size:%d\n", width);
    // Rows can start with even pattern in case in prev rows there where odd number of patters.
    // So lets try twice
    _pairs.clear();
    _startFromEven = false;
    try {
        //printf("RSSExpandedReader::decodeRow(%d) Odd\n", rowNumber);
        std::vector< Ref<ExpandedPair> > pairs = decodeRow2pairs(rowNumber, row);
        Ref<Result> result = constructResult(pairs);
        this->reset();
        return result;
    } catch (NotFoundException e) {
        // OK
    }
    
    _pairs.clear();
    _startFromEven = true;
    //printf("RSSExpandedReader::decodeRow(%d) Even\n", rowNumber);
    std::vector< Ref<ExpandedPair> > pairs = decodeRow2pairs(rowNumber, row);
    Ref<Result> result = constructResult(pairs);
    this->reset();
    return result;
}

void RSSExpandedReader::reset() {
    _pairs.clear();
    _rows.clear();
}

std::vector< Ref<ExpandedPair> > RSSExpandedReader::decodeRow2pairs(int rowNumber, Ref<BitArray> row) {
    try {
        while (true){
            Ref<ExpandedPair> nextPair = retrieveNextPair(row, _pairs, rowNumber);
            _pairs.push_back(nextPair);
            //System.out.println(this.pairs.size()+" pairs found so far on row "+rowNumber+": "+this.pairs);
            // exit this loop when retrieveNextPair() fails and throws
        }
    } catch (NotFoundException nfe) {
        if (_pairs.empty()) {
            throw nfe;
        }
    }
    
    //printf("RSSExpandedReader::decodeRow2Pair(rowNumber[%d], rowCnt[%d]) _rows:%d _pairs:%d\n", rowNumber, row->getSize(), (int)_rows.size(), (int)_pairs.size());
    
    // TODO: verify sequence of finder patterns as in checkPairSequence()
    if (checkChecksum()) {
        return _pairs;
    }
    
    boolean tryStackedDecode = !_rows.empty();
    boolean wasReversed = false; // TODO: deal with reversed rows
    storeRow(rowNumber, wasReversed);
    if (tryStackedDecode) {
        // When the image is 180-rotated, then rows are sorted in wrong dirrection.
        // Try twice with both the directions.
        //printf("decodeRow2Pair() tryStackedDecode\n");
        std::vector< Ref<ExpandedPair> > ps = checkRows(false);
        if (!ps.empty()) {
            return ps;
        }
        ps = checkRows(true);
        if (!ps.empty()) {
            return ps;
        }
    }
    
    throw NotFoundException();
}

std::vector< Ref<ExpandedPair> > RSSExpandedReader::checkRows(bool reverse) {
    
    // Limit number of rows we are checking
    // We use recursive algorithm with pure complexity and don't want it to take forever
    // Stacked barcode can have up to 11 rows, so 25 seems resonable enough
    if (_rows.size() > 25) {
        _rows.clear(); // We will never have a chance to get result, so clear it
        return std::vector< Ref<ExpandedPair> >();
    }
    
    _pairs.clear();
    if (reverse) {
        for (int i = 0, j = _rows.size() - 1; i < j; i++, j--) {
            Ref<ExpandedRow> temp = _rows[i];
            _rows[i] = _rows[j];
            _rows[j] = temp;
        }
    }
    
    std::vector< Ref<ExpandedPair> > ps;
    try {
        std::vector< Ref<ExpandedRow> > rows;
        ps = checkRows(rows, 0);
    } catch (NotFoundException e) {
        // OK
    }
    
    if (reverse) {
        for (int i = 0, j = _rows.size() - 1; i < j; i++, j--) {
            Ref<ExpandedRow> temp = _rows[i];
            _rows[i] = _rows[j];
            _rows[j] = temp;
        }
    }
    
    return ps;
}

std::vector< Ref<ExpandedPair> > RSSExpandedReader::checkRows(std::vector< Ref<ExpandedRow> > &collectedRows, int currentRow) {
    // Try to construct a valid rows sequence
    // Recursion is used to implement backtracking
    int rowSize = _rows.size();
    
    if (rowSize == 0) {
        throw NotFoundException();
    }
    
    for (int i = currentRow; i < rowSize; i++) {
        Ref<ExpandedRow> row = _rows[i];
        _pairs.clear();
        int size = collectedRows.size();
        for (int j = 0; j < size; j++) {
            std::vector< Ref<ExpandedPair> > &collectedPairs = collectedRows[j]->getPairs();
            if (!collectedPairs.empty()) {
                _pairs.insert(_pairs.end(), collectedPairs.begin(), collectedPairs.end());
            }
        }
        std::vector< Ref<ExpandedPair> > &pairs = row->getPairs();
        if (!pairs.empty()) {
            _pairs.insert(_pairs.end(), pairs.begin(), pairs.end());
        }
        
        if (!isValidSequence(_pairs)) {
            continue;
        }
        
        if (checkChecksum()) {
            return _pairs;
        }
        
        std::vector< Ref<ExpandedRow> > rs(collectedRows);
        rs.push_back(row);
        try {
            // Recursion: try to add more rows
            return checkRows(rs, i + 1);
        } catch (NotFoundException e) {
            // We failed, try the next candidate
        }
    }
    
    throw NotFoundException();
}

bool RSSExpandedReader::isValidSequence(std::vector< Ref<ExpandedPair> > &pairs) {
    for (std::vector< std::vector<int> >::const_iterator it = FINDER_PATTERN_SEQUENCES.begin(); it != FINDER_PATTERN_SEQUENCES.end(); it++) {
        std::vector<int> sequence = *it;
        if (pairs.size() > sequence.size()) {
            continue;
        }
        
        boolean stop = true;
        int pairSize = pairs.size();
        for (int j = 0; j < pairSize; j++) {
            if (pairs[j]->getFinderPattern()->getValue() != sequence[j]) {
                stop = false;
                break;
            }
        }
        
        if (stop) {
            return true;
        }
    }
    
    return false;
}

void RSSExpandedReader::storeRow(int rowNumber, bool wasReversed) {
    // Discard if duplicate above or below; otherwise insert in order by row number.
    int insertPos = 0;
    boolean prevIsSame = false;
    boolean nextIsSame = false;
    int rowSize = _rows.size();
    while (insertPos < rowSize) {
        Ref<ExpandedRow> erow = _rows[insertPos];
        if (erow->getRowNumber() > rowNumber) {
            nextIsSame = erow->isEquivalent(_pairs);
            break;
        }
        prevIsSame = erow->isEquivalent(_pairs);
        insertPos++;
    }
    if (nextIsSame || prevIsSame) {
        return;
    }
    
    // When the row was partially decoded (e.g. 2 pairs found instead of 3),
    // it will prevent us from detecting the barcode.
    // Try to merge partial rows
    
    // Check whether the row is part of an allready detected row
    if (isPartialRow(_pairs, _rows)) {
        return;
    }
    
    std::vector< Ref<ExpandedPair> > pairs(_pairs); // Create a copy
    //printf("storeRow() p[%d] [%d]\n", (int)_pairs.size(), (int)pairs.size());

    Ref<ExpandedRow> row(new ExpandedRow(pairs, rowNumber, wasReversed));
    _rows.insert(_rows.begin() + insertPos, row);
    
    removePartialRows(pairs, _rows);
}

void RSSExpandedReader::removePartialRows(std::vector< Ref<ExpandedPair> > &pairs, std::vector< Ref<ExpandedRow> > &rows) {
    // Remove all the rows that contains only specified pairs
    std::vector< Ref<ExpandedRow> >::iterator iterator = rows.begin();
    while (iterator != rows.end()) {
        Ref<ExpandedRow> r = *iterator;
        if (r->getPairs().size() == pairs.size()) {
            ++iterator;
            continue;
        }
        boolean allFound = true;
        std::vector< Ref<ExpandedPair> > &rPairs = r->getPairs();
        for (std::vector< Ref<ExpandedPair> >::iterator it = rPairs.begin(); it != rPairs.end(); it++) {
            Ref<ExpandedPair> p = *it;
            boolean found = false;
            for (std::vector< Ref<ExpandedPair> >::iterator itp = pairs.begin(); itp != pairs.end(); itp++) {
                Ref<ExpandedPair> pp = *itp;
                if (p->equals(pp)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                allFound = false;
                break;
            }
        }
        if (allFound) {
            // 'pairs' contains all the pairs from the row 'r'
            iterator = rows.erase(iterator);
            //printf("removePartialRows() - rows.erase()");
        } else {
            ++iterator;
        }
    }
}

bool RSSExpandedReader::isPartialRow(std::vector< Ref<ExpandedPair> > &pairs, std::vector< Ref<ExpandedRow> > &rows) {
    // Returns true when one of the rows already contains all the pairs
    for (std::vector< Ref<ExpandedRow> >::iterator it = rows.begin(); it != rows.end(); it++) {
        Ref<ExpandedRow> r = *it;
        boolean allFound = true;
        for (std::vector< Ref<ExpandedPair> >::iterator itp = pairs.begin(); itp != pairs.end(); itp++) {
            Ref<ExpandedPair> p = *itp;
            boolean found = false;
            std::vector< Ref<ExpandedPair> > &rPairs = r->getPairs();
            for (std::vector< Ref<ExpandedPair> >::iterator itpp = rPairs.begin(); itpp != rPairs.end(); itpp++) {
                Ref<ExpandedPair> pp = *itpp;
                if (p->equals(pp)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                allFound = false;
                break;
            }
        }
        if (allFound) {
            // the row 'r' contain all the pairs from 'pairs'
            return true;
        }
    }
    return false;
}

Ref<Result> RSSExpandedReader::constructResult(std::vector< Ref<ExpandedPair> > &pairs) {
    Ref<BitArray> binary = BitArrayBuilder::buildBitArray(pairs);
    
    Ref<AbstractExpandedDecoder> decoder = AbstractExpandedDecoder::createDecoder(binary);
    Ref<String> resultingString = decoder->parseInformation();
    
    ArrayRef< Ref<ResultPoint> > firstPoints = pairs[0]->getFinderPattern()->getResultPoints();
    ArrayRef< Ref<ResultPoint> > lastPoints  = pairs[pairs.size() - 1]->getFinderPattern()->getResultPoints();
    
    Array< Ref<ResultPoint> >* resultPoints = new Array< Ref<ResultPoint> >();
    std::vector< Ref<ResultPoint> >& resultPointValues (resultPoints->values());
    resultPointValues.push_back(Ref<ResultPoint>(firstPoints[0]));
    resultPointValues.push_back(Ref<ResultPoint>(firstPoints[1]));
    resultPointValues.push_back(Ref<ResultPoint>(lastPoints[0]));
    resultPointValues.push_back(Ref<ResultPoint>(lastPoints[1]));
    
    Result *res = new Result(resultingString,
                             ArrayRef<char>(),
                             ArrayRef< Ref<ResultPoint> >(resultPoints),
                             BarcodeFormat::RSS_EXPANDED
                             );
    return Ref<Result>(res);
}

bool RSSExpandedReader::checkChecksum() {
    if (_pairs.empty()) {
        return false;
    }
    Ref<ExpandedPair> firstPair = _pairs[0];;
    Ref<DataCharacter> checkCharacter = firstPair->getLeftChar();
    Ref<DataCharacter> firstCharacter = firstPair->getRightChar();
    
    if (checkCharacter.empty() || firstCharacter.empty()) {
        return false;
    }
    
    int checksum = firstCharacter->getChecksumPortion();
    int s = 2;
    
    int pairSize = _pairs.size();
    for(int i = 1; i < pairSize; ++i){
        Ref<ExpandedPair> currentPair = _pairs[i];
        checksum += currentPair->getLeftChar()->getChecksumPortion();
        s++;
        Ref<DataCharacter> currentRightChar = currentPair->getRightChar();
        if (!currentRightChar.empty()) {
            checksum += currentRightChar->getChecksumPortion();
            s++;
        }
    }
    
    checksum %= 211;
    
    int checkCharacterValue = 211 * (s - 4) + checksum;
//    if (checkCharacterValue == checkCharacter->getValue()) {
        //printf("RSSExpandedReader::checkChecksum() %d == %d s[%d] cs[%d] _rows[%d] _pairs[%d]\n", checkCharacterValue, checkCharacter->getValue(), s, checksum, (int)_rows.size(), (int)_pairs.size());
//    }
    return checkCharacterValue == checkCharacter->getValue();
}

int RSSExpandedReader::getNextSecondBar(Ref<BitArray> row, int initialPos) {
    int currentPos;
    if (row->get(initialPos)) {
        currentPos = row->getNextUnset(initialPos);
        currentPos = row->getNextSet(currentPos);
    } else {
        currentPos = row->getNextSet(initialPos);
        currentPos = row->getNextUnset(currentPos);
    }
    return currentPos;
}

Ref<ExpandedPair> RSSExpandedReader::retrieveNextPair(Ref<BitArray> row, std::vector< Ref<ExpandedPair> > &previousPairs, int rowNumber) {
    bool isOddPattern  = previousPairs.size() % 2 == 0;
    if (_startFromEven) {
        isOddPattern = !isOddPattern;
    }
    
    Ref<FinderPattern> pattern;
    
    bool keepFinding = true;
    int forcedOffset = -1;
    do{
        findNextPair(row, previousPairs, forcedOffset);
        pattern = parseFoundFinderPattern(row, rowNumber, isOddPattern);
        if (pattern.empty()){
            forcedOffset = RSSExpandedReader::getNextSecondBar(row, _startEnd[0]);
        } else {
            keepFinding = false;
        }
    }while(keepFinding);
    
    // When stacked symbol is split over multiple rows, there's no way to guess if this pair can be last or not.
    // boolean mayBeLast = checkPairSequence(previousPairs, pattern);
    
    Ref<DataCharacter> leftChar  = decodeDataCharacter(row, pattern, isOddPattern, true);
    
    if (!previousPairs.empty() && previousPairs[previousPairs.size()-1]->mustBeLast()) {
        throw NotFoundException();
    }
    
    Ref<DataCharacter> rightChar;
    try {
        rightChar = decodeDataCharacter(row, pattern, isOddPattern, false);
    } catch(NotFoundException ignored) {
        rightChar = Ref<DataCharacter>();
    }
    bool mayBeLast = true;
    return Ref<ExpandedPair>(new ExpandedPair(leftChar, rightChar, pattern, mayBeLast));
}

void RSSExpandedReader::findNextPair(Ref<BitArray> row, std::vector< Ref<ExpandedPair> > &previousPairs, int forcedOffset) {
    std::vector<int> &counters = getDecodeFinderCounters();
    counters[0] = 0;
    counters[1] = 0;
    counters[2] = 0;
    counters[3] = 0;
    
    int width = row->getSize();
    
    int rowOffset;
    if (forcedOffset >= 0) {
        rowOffset = forcedOffset;
    } else if (previousPairs.empty()) {
        rowOffset = 0;
    } else{
        Ref<ExpandedPair> lastPair = previousPairs[previousPairs.size() - 1];
        rowOffset = lastPair->getFinderPattern()->getStartEnd()[1];
    }
    bool searchingEvenPair = previousPairs.size() % 2 != 0;
    if (_startFromEven) {
        searchingEvenPair = !searchingEvenPair;
    }
    
    bool isWhite = false;
    while (rowOffset < width) {
        isWhite = !row->get(rowOffset);
        if (!isWhite) {
            break;
        }
        rowOffset++;
    }
    
    int counterPosition = 0;
    int patternStart = rowOffset;
    for (int x = rowOffset; x < width; x++) {
        if (row->get(x) ^ isWhite) {
            counters[counterPosition]++;
        } else {
            if (counterPosition == 3) {
                if (searchingEvenPair) {
                    reverseCounters(counters);
                }
                
                if (isFinderPattern(counters)){
                    _startEnd[0] = patternStart;
                    _startEnd[1] = x;
                    return;
                }
                
                if (searchingEvenPair) {
                    reverseCounters(counters);
                }
                
                patternStart += counters[0] + counters[1];
                counters[0] = counters[2];
                counters[1] = counters[3];
                counters[2] = 0;
                counters[3] = 0;
                counterPosition--;
            } else {
                counterPosition++;
            }
            counters[counterPosition] = 1;
            isWhite = !isWhite;
        }
    }
    throw NotFoundException();
}

void RSSExpandedReader::reverseCounters(std::vector<int> &counters) {
    int length = counters.size();
    for(int i = 0; i < length / 2; ++i){
        int tmp = counters[i];
        counters[i] = counters[length - i - 1];
        counters[length - i - 1] = tmp;
    }
}

Ref<FinderPattern> RSSExpandedReader::parseFoundFinderPattern(Ref<BitArray> row, int rowNumber, bool oddPattern) {
    // Actually we found elements 2-5.
    int firstCounter;
    int start;
    int end;
    
    if(oddPattern){
        // If pattern number is odd, we need to locate element 1 *before* the current block.
        
        int firstElementStart = _startEnd[0] - 1;
        // Locate element 1
        while (firstElementStart >= 0 && !row->get(firstElementStart)) {
            firstElementStart--;
        }
        
        firstElementStart++;
        firstCounter = _startEnd[0] - firstElementStart;
        start = firstElementStart;
        end = _startEnd[1];
        
    }else{
        // If pattern number is even, the pattern is reversed, so we need to locate element 1 *after* the current block.
        
        start = _startEnd[0];
        
        end = row->getNextUnset(_startEnd[1] + 1);
        firstCounter = end - _startEnd[1];
    }
    
    // Make 'counters' hold 1-4
    std::vector<int> &counters = getDecodeFinderCounters();
    std::vector<int> countersCopy(counters.begin(),counters.end());
    int counterCount = counters.size();
    
    for (int y = 0; y < counterCount - 1; y++) {
        counters[y + 1] = countersCopy[y];
    }
    
    counters[0] = firstCounter;
    int value;
    try {
        value = parseFinderValue(counters, FINDER_PATTERNS);
    } catch (NotFoundException ignored) {
        return Ref<FinderPattern>();
    }
    int rangeArr[] = {start, end};
    std::vector<int> range (rangeArr, rangeArr + sizeof(rangeArr) / sizeof(rangeArr[0]));
    return Ref<FinderPattern>(new FinderPattern(value, range, start, end, rowNumber));
}

Ref<DataCharacter> RSSExpandedReader::decodeDataCharacter(Ref<BitArray> row, Ref<FinderPattern> pattern, bool isOddPattern, bool leftChar) {
    std::vector<int> &counters = getDataCharacterCounters();
    counters[0] = 0;
    counters[1] = 0;
    counters[2] = 0;
    counters[3] = 0;
    counters[4] = 0;
    counters[5] = 0;
    counters[6] = 0;
    counters[7] = 0;
    
    if (leftChar) {
        recordPatternInReverse(row, pattern->getStartEnd()[0], counters);
    } else {
        recordPattern(row, pattern->getStartEnd()[1], counters);
        // reverse it
        for (int i = 0, j = counters.size() - 1; i < j; i++, j--) {
            int temp = counters[i];
            counters[i] = counters[j];
            counters[j] = temp;
        }
    }//counters[] has the pixels of the module
    
    int numModules = 17; //left and right data characters have all the same length
    float elementWidth = (float) count(counters) / (float) numModules;
    
    // Sanity check: element width for pattern and the character should match
    float expectedElementWidth = (pattern->getStartEnd()[1] - pattern->getStartEnd()[0]) / 15.0f;
    if (std::abs(elementWidth - expectedElementWidth) / expectedElementWidth > 0.3f) {
        throw NotFoundException();
    }
    
    std::vector<int> &oddCounts = getOddCounts();
    std::vector<int> &evenCounts = getEvenCounts();
    std::vector<float> &oddRoundingErrors = getOddRoundingErrors();
    std::vector<float> &evenRoundingErrors = getEvenRoundingErrors();
    
    for (int i = 0; i < (int)counters.size(); i++) {
        float value = 1.0f * counters[i] / elementWidth;
        int count = (int) (value + 0.5f); // Round
        if (count < 1) {
            if (value < 0.3f) {
                throw NotFoundException();
            }
            count = 1;
        } else if (count > 8) {
            if (value > 8.7f) {
                throw NotFoundException();
            }
            count = 8;
        }
        int offset = i >> 1;
        if ((i & 0x01) == 0) {
            oddCounts[offset] = count;
            oddRoundingErrors[offset] = value - count;
        } else {
            evenCounts[offset] = count;
            evenRoundingErrors[offset] = value - count;
        }
    }
    
    adjustOddEvenCounts(numModules);
    
    int weightRowNumber = 4 * pattern->getValue() + (isOddPattern?0:2) + (leftChar?0:1) - 1;
    
    int oddSum = 0;
    int oddChecksumPortion = 0;
    for (int i = oddCounts.size() - 1; i >= 0; i--) {
        if(RSSExpandedReader::isNotA1left(pattern, isOddPattern, leftChar)){
            int weight = WEIGHTS[weightRowNumber][2 * i];
            oddChecksumPortion += oddCounts[i] * weight;
        }
        oddSum += oddCounts[i];
    }
    int evenChecksumPortion = 0;
    //int evenSum = 0;
    for (int i = evenCounts.size() - 1; i >= 0; i--) {
        if(RSSExpandedReader::isNotA1left(pattern, isOddPattern, leftChar)){
            int weight = WEIGHTS[weightRowNumber][2 * i + 1];
            evenChecksumPortion += evenCounts[i] * weight;
        }
        //evenSum += evenCounts[i];
    }
    int checksumPortion = oddChecksumPortion + evenChecksumPortion;
    
    if ((oddSum & 0x01) != 0 || oddSum > 13 || oddSum < 4) {
        throw NotFoundException();
    }
    
    int group = (13 - oddSum) / 2;
    int oddWidest = SYMBOL_WIDEST[group];
    int evenWidest = 9 - oddWidest;
    int vOdd = RSSUtils::getRSSvalue(oddCounts, oddWidest, true);
    int vEven = RSSUtils::getRSSvalue(evenCounts, evenWidest, false);
    int tEven = EVEN_TOTAL_SUBSET[group];
    int gSum = GSUM[group];
    int value = vOdd * tEven + vEven + gSum;
    
    return Ref<DataCharacter>(new DataCharacter(value, checksumPortion));
}

bool RSSExpandedReader::isNotA1left(Ref<FinderPattern> pattern, bool isOddPattern, bool leftChar) {
    // A1: pattern.getValue is 0 (A), and it's an oddPattern, and it is a left char
    return !(pattern->getValue() == 0 && isOddPattern && leftChar);
}

void RSSExpandedReader::adjustOddEvenCounts(int numModules) {
    int oddSum = count(getOddCounts());
    int evenSum = count(getEvenCounts());
    int mismatch = oddSum + evenSum - numModules;
    boolean oddParityBad = (oddSum & 0x01) == 1;
    boolean evenParityBad = (evenSum & 0x01) == 0;
    
    boolean incrementOdd = false;
    boolean decrementOdd = false;
    
    if (oddSum > 13) {
        decrementOdd = true;
    } else if (oddSum < 4) {
        incrementOdd = true;
    }
    boolean incrementEven = false;
    boolean decrementEven = false;
    if (evenSum > 13) {
        decrementEven = true;
    } else if (evenSum < 4) {
        incrementEven = true;
    }
    
    if (mismatch == 1) {
        if (oddParityBad) {
            if (evenParityBad) {
                throw NotFoundException();
            }
            decrementOdd = true;
        } else {
            if (!evenParityBad) {
                throw NotFoundException();
            }
            decrementEven = true;
        }
    } else if (mismatch == -1) {
        if (oddParityBad) {
            if (evenParityBad) {
                throw NotFoundException();
            }
            incrementOdd = true;
        } else {
            if (!evenParityBad) {
                throw NotFoundException();
            }
            incrementEven = true;
        }
    } else if (mismatch == 0) {
        if (oddParityBad) {
            if (!evenParityBad) {
                throw NotFoundException();
            }
            // Both bad
            if (oddSum < evenSum) {
                incrementOdd = true;
                decrementEven = true;
            } else {
                decrementOdd = true;
                incrementEven = true;
            }
        } else {
            if (evenParityBad) {
                throw NotFoundException();
            }
            // Nothing to do!
        }
    } else {
        throw NotFoundException();
    }
    
    if (incrementOdd) {
        if (decrementOdd) {
            throw NotFoundException();
        }
        std::vector<int> &oddCounts = getOddCounts();
        std::vector<float> &oddRoundingErrors = getOddRoundingErrors();
        increment(oddCounts, oddRoundingErrors);
    }
    if (decrementOdd) {
        std::vector<int> &oddCounts = getOddCounts();
        std::vector<float> &oddRoundingErrors = getOddRoundingErrors();
        decrement(oddCounts, oddRoundingErrors);
    }
    if (incrementEven) {
        if (decrementEven) {
            throw NotFoundException();
        }
        std::vector<int> &evenCounts = getEvenCounts();
        std::vector<float> &oddRoundingErrors = getOddRoundingErrors();
        increment(evenCounts, oddRoundingErrors);
    }
    if (decrementEven) {
        std::vector<int> &evenCounts = getEvenCounts();
        std::vector<float> &evenRoundingErrors = getEvenRoundingErrors();
        decrement(evenCounts, evenRoundingErrors);
    }
}




