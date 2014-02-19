-- Papa Florin, 324CA

module Regexp where

import Parser
import Data.Char

-- tipul de date Regex care contine constructori pentru toate situatiile ce pot fi intalnite la parsare
data Regex = Letter Char | Number Char | Sequence Regex Regex | Alternation Regex Regex | Optional Regex | ZeroOrMore Regex | Group Regex	| Plus Regex | CuantFix Regex Regex | CuantLeast Regex Regex| CuantBetween Regex Regex Regex deriving Show

-- contine toate parserele posibile si se aplica pe expresia regulata
expression =  parseGroup `alt` parseAlternation `alt` parseLetter `alt` parseSequence `alt` parseOptional `alt` parseZeroOrMore `alt` parsePlus	`alt` parseFix `alt` parseBetween `alt` parseLeast

seqTerm = parseLetter `alt` parseOptional `alt` parseZeroOrMore `alt` parseGroup `alt` parsePlus `alt` parseFix `alt` parseBetween `alt` parseLeast
seqTerm2 = parseGroup `alt` parseLetter `alt` parseSequence `alt` parseOptional `alt` parseZeroOrMore `alt` parsePlus `alt` parseFix `alt` parseBetween `alt` parseLeast
altTerm = parseLetter `alt` parseSequence `alt` parseGroup `alt` parsePlus `alt` parseOptional `alt` parseZeroOrMore `alt` parseFix `alt` parseBetween `alt` parseLeast
optTerm = parseLetter `alt` parseGroup

-- parsarea expresiei regulate pentru obtinerea unei structuri arborescente, usor de procesat mai departe
-- fiecare parser are un domeniu propriu de parsere posibile, in functie de prioritati si functionalitati
parseLetter = transform (spot isLetter) Letter
parseNumber = transform (spot isDigit) Number
parseSequence = transform (seqTerm >*> seqTerm2) (\(x, y) -> Sequence x y)
parseAlternation = transform (altTerm >*> (token '|') >*> expression) (\(a, (x, y)) -> Alternation a y)
parseOptional = transform (optTerm >*> (token '?')) (\(x, y) -> Optional x)
parseZeroOrMore = transform (optTerm >*> (token '*')) (\(x, y) -> ZeroOrMore x)
parseGroup = transform ((token '(') >*> expression >*> (token ')')) (\(a, (x, y)) -> Group x)
parsePlus = transform (optTerm >*> (token '+')) (\(x, y) -> Plus x)
parseFix = transform (optTerm >*> (token '{') >*> parseNumber >*> (token '}')) (\(a, (b, (c, d))) -> CuantFix a c)
parseLeast = transform (optTerm >*> (token '{') >*> parseNumber >*> (token ',') >*> (token '}')) (\(a, (b, (c, (d, e)))) -> CuantLeast a c)
parseBetween = transform (optTerm >*> (token '{') >*> parseNumber >*> (token ',') >*> parseNumber >*> (token '}')) (\(a, (b, (c, (d, (e, f))))) -> CuantBetween a c e)


--parsarea stringului in care se cauta potriviri
process (Letter a) = transform (token a) (\a -> [a])
process (Sequence r1 r2) = transform ((process r1) >*> (process r2)) (\(a, b) -> a ++ b)
process (Alternation r1 r2) = (process r1) `alt` (process r2)
process (Optional r) = (process r) `alt` (success "")
process (ZeroOrMore r) = process (Sequence r (ZeroOrMore r)) `alt` (success "")
process (Group r) = process r
process (Plus r) = process (Sequence r (ZeroOrMore r))
process (CuantFix r (Number '1')) = process r
process(CuantFix r (Number x)) = process(Sequence r (CuantFix r (Number x2))) where x2 = (intToDigit ((digitToInt x) - 1))
process(CuantLeast r (Number '0')) = process(ZeroOrMore r)
process(CuantLeast r (Number x)) = process(Sequence (CuantFix r (Number x)) (ZeroOrMore r))
process(CuantBetween r (Number '0') (Number '0')) = (success "")
process(CuantBetween r (Number '0') (Number x)) = process(Sequence (Optional r) (CuantBetween r (Number '0') (Number x2))) where x2 = (intToDigit ((digitToInt x) - 1))
process(CuantBetween r (Number x) (Number y)) = process(Sequence (CuantFix r (Number x)) (CuantBetween r (Number '0') (Number '1'))) where z = (intToDigit ((digitToInt y) - (digitToInt x)))

-- intoarce cea mai lunga potrivire dintr-o lista de potriviri 
getLongestString l max rez
	| l == [] = rez
	| otherwise = if(length (fst (head l)) > max) then getLongestString (tail l) (length (fst (head l))) (fst (head l)) else (getLongestString (tail l) max rez) 

-- intoarce structura arborescenta a expresiei regulate
getRegex x = result expression x

-- intoarce potrivirile incepand cu primul caracter al sirului y
getFirstMatch x y = process (getRegex x) y

-- intoarce cea mai lunga potrivire incepand cu primul caracter al sirului y
getLongestMatch x y = getLongestString (getFirstMatch x y) 0 []

-- functia matches
matches a b
	| ((b == []) && ((getFirstMatch a b) == [])) = []
	| b == [] = [getLongestMatch a b]
	| ((getFirstMatch a b) == []) = (matches a (tail b))
	| (length (getLongestMatch a b)) == 0 = [(getLongestMatch a b)] ++ (matches a (tail b))	 
	| otherwise = [(getLongestMatch a b)] ++ (matches a (drop (length (getLongestMatch a b)) b))
	 	