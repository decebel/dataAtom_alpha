#### PATTERN | EN | QUANTIFY #######################################################################
# Copyright (c) 2010 University of Antwerp, Belgium
# Author: Tom De Smedt <tom@organisms.be>
# License: BSD (see LICENSE.txt for details).

####################################################################################################
# Transforms numeral strings to numbers, and numbers (int, float) to numeral strings.
# Approximates quantities of objects ("dozens of chickens" etc.)

import re
from math import log, ceil

try: from pattern.en.inflect import pluralize, referenced
except:
    try: 
        from inflect import pluralize, referenced
    except:
        from __init__ import pluralize, referenced

NUMERALS = {
    "zero"      :  0, 
    "one"       :  1, 
    "two"       :  2, 
    "three"     :  3, 
    "four"      :  4, 
    "five"      :  5, 
    "six"       :  6, 
    "seven"     :  7,
    "eight"     :  8,
    "nine"      :  9, 
    "ten"       : 10, 
    "eleven"    : 11, 
    "twelve"    : 12,
    "thirteen"  : 13, 
    "fourteen"  : 14, 
    "fifteen"   : 15,
    "sixteen"   : 16, 
    "seventeen" : 17, 
    "eighteen"  : 18, 
    "nineteen"  : 19,
    "twenty"    : 20, 
    "thirty"    : 30, 
    "forty"     : 40, 
    "fifty"     : 50, 
    "sixty"     : 60, 
    "seventy"   : 70, 
    "eighty"    : 80, 
    "ninety"    : 90
}
# Integer keys pointing to numeral words:
NUMERALS_REVERSED = dict([(b,a) for a,b in NUMERALS.items()])

# Some special numerals:
# first multiply by the second value, then add the first value
# (e.g. two dozen => 2x12, two and a half => 2*1 + 0.5)
NUMERALS_ALTERNATE = {
    "half"  : (0.5, 1),
    "dozen" : (0, 12),
    "score" : (0, 20)
}

ORDER = [
    "hundred",
    "thousand"        
] + [m+"illion" for m in [
    "m", 
    "b", 
    "tr", 
    "quadr", 
    "quint",
    "sext",
    "sept",
    "oct",
    "non",
    "dec",
    "undec",
    "duodec",
    "tredec",
    "quattuordec",
    "quindec",
    "sexdec",
    "septemdec",
    "octodec",
    "novemdec",
    "vigint" # A one + 62 zeroes; quite big but not nearly a googol.
]]

# Dictionary of thousands: {"hundred": 100, "thousand": 1000, etc.}
O = {ORDER[0]:100, ORDER[1]:1000}
for i, k in enumerate(ORDER[2:]): O[k] = 1000000*1000**i

ZERO        = "zero"
MINUS       = "minus"
RADIX       = "point"
THOUSANDS   = ","
CONJUNCTION = "and"

def _extract_leading_zeros(string):
    """ Returns a tuple of 1) the string with leading zeros stripped and 2) a count of zeros.
        For example:
        zero zero one => ("one", 2)
        0 0 five => ("five", 2)
    """
    i = 0
    string = string.lstrip()
    while True:
        if string.startswith(ZERO):
            string = string.replace(ZERO,"",1).lstrip()
            i+=1
        elif string.startswith("0"):
            string = string.replace("0","",1).lstrip()
            i+=1
        else:
            break
    return string.lstrip(), i

#--- STRING TO NUMBER ------------------------------------------------------------------------------

def number(string):
    """ Returns the given numeric string as a float or an int.
        If no number can be parsed from the string, returns 0.
        For example:
        number("five point two million") => 5200000
        number("seventy-five point two") => 75.2
        number("three thousand and one") => 3001
    """
    string = string.lower()
    # For negative numbers, simply prepend minus.
    if string.strip().startswith(MINUS):
        return -number(string.strip().replace(MINUS,"",1))
    # Strip commas and dashes ("seventy-five").
    # Split into integral and fractional part.
    string = string.replace("&", " %s " % CONJUNCTION)
    string = string.replace(THOUSANDS, "").replace("-", " ")
    string = string.split(RADIX)
    # Process fractional part.
    # Extract all the leading zeros.
    if len(string) > 1:
        f = " ".join(string[1:])         # zero point zero twelve => zero twelve
        f, z = _extract_leading_zeros(f) # zero twelve => (1, "twelve")
        f = float(number(f))             # "twelve" => 12.0
        f /= 10**(len(str(int(f)))+z)    # 10**(len("12")+1) = 1000; 12.0 / 1000 => 0.012
    else:
        f = 0
    i = n = 0
    string = string[0].split()
    for j, x in enumerate(string):
        if x in NUMERALS:
            # Map words from the dictionary of numerals: "eleven" => 11.
            i += NUMERALS[x]
        elif x in NUMERALS_ALTERNATE:
            # Map words from alternate numerals: "two dozen" => (2+0)*12
            i = i * NUMERALS_ALTERNATE[x][1] + NUMERALS_ALTERNATE[x][0]
        elif x in O: 
            # Map thousands from the dictionary of orders.
            # When a thousand is encountered, the subtotal is shifted to the total
            # and we start a new subtotal. An exception to this is when we
            # encouter two following thousands (e.g. two million vigintillion is one subtotal).
            i *= O[x]
            if j < len(string)-1 and string[j+1] in O: 
                continue
            if O[x] > 100: 
                n += i
                i = 0
        elif x == CONJUNCTION:
            pass
        else:
            # Words that are not in any dicionary may be numbers (e.g. "2.5" => 2.5).
            try: i += "." in x and float(x) or int(x)
            except:
                pass
    return n + i + f

#print number("five point two septillion")
#print number("seventy-five point two")
#print number("three thousand and one")
#print number("1.2 million point two")
#print number("nothing")

#--- NUMBER TO STRING ------------------------------------------------------------------------------

def numerals(n, round=2):
    """ Returns the given int or float as a string of numerals.
        By default, the fractional part is rounded to two decimals.
        For example:
        numerals(4011) => four thousand and eleven
        numerals(2.25) => two point twenty-five
        numerals(2.249) => two point twenty-five
        numerals(2.249, round=3) => two point two hundred and forty-nine
        Note: due to rounding of float values, float(number(x)) == x is not always True.
    """
    if isinstance(n, basestring):
        if n.isdigit():
            n = int(n)
        else:
            # If the float is given as a string, extract the length of the fractional part.
            if round is None:
                round = len(n.split(".")[1])
            n = float(n)
    # For negative numbers, simply prepend minus.
    if n < 0:
        return "%s %s" % (MINUS, numerals(abs(n)))
    # Split the number into integral and fractional part.
    # Converting the integral part to a long ensures a better accuracy during the recursion.
    i = long(n//1)
    f = n-i
    # The remainder, which we will stringify in recursion.
    r = 0
    if i in NUMERALS_REVERSED: # 11 => eleven
        # Map numbers from the dictionary to numerals: 11 => "eleven".
        s = NUMERALS_REVERSED[i]
    elif i < 100:
        # Map tens + digits: 75 => 70+5 => "seventy-five".
        s = numerals((i//10)*10) + "-" + numerals(i%10)
    elif i < 1000:
        # Map hundreds: 500 => 5*100 => "five hundred".
        # Store the remainders (tens + digits).
        s = numerals(i//100) + " " + ORDER[0]
        r = i % 100
    else:
        # Map thousands by extracting the order (thousand/million/billion/...).
        # Store and recurse the remainder.
        s = ""
        o, base = 1, 1000
        while i > base:
            o+=1; base*=1000
        while o > len(ORDER)-1:
            s += " "+ORDER[-1] # This occurs for consecutive thousands: million vigintillion.
            o -= len(ORDER)-1
        s = "%s %s%s" % (numerals(i//(base/1000)), (o>1 and ORDER[o-1] or ""), s)
        r = i % (base/1000)
    if f != 0: 
        # Map the fractional part: "two point twenty-five" => 2.25.
        # We cast it to a string first to find all the leading zeros.
        # This actually seems more accurate than calculating the leading zeros,
        # see also: http://python.org/doc/2.5.1/tut/node16.html.
        # Some rounding occurs.
        f = ("%." + str(round is None and 2 or round) + "f") % f
        f = f.replace("0.","",1).rstrip("0")
        f, z = _extract_leading_zeros(f)
        f = f and " %s%s %s" % (RADIX, " %s"%ZERO*z, numerals(long(f))) or ""
    else:
        f = ""
    if r == 0:
        return s+f
    elif r >= 1000: 
        # Separate hundreds and thousands with a comma: two million, three hundred thousand.
        return "%s%s %s" % (s, THOUSANDS, numerals(r)+f)
    elif r <= 100:  
        # Separate hundreds and tens with "and": two thousand three hundred and five.
        return "%s %s %s" % (s, CONJUNCTION, numerals(r)+f)
    else:
        return "%s %s" % (s, numerals(r)+f)

#--- APPROXIMATE -----------------------------------------------------------------------------------
# Based on the Ruby Linguistics module by Michael Granger:
# http://www.deveiate.org/projects/Linguistics/wiki/English

NONE      = "no"          #  0
PAIR      = "a pair of"   #  2
SEVERAL   = "several"     #  3-7
NUMBER    = "a number of" #  8-17
SCORE     = "a score of"  # 18-22
DOZENS    = "dozens of"   # 22-200
COUNTLESS = "countless"

quantify_custom_plurals = {}

def approximate(word, amount=1, plural={}):
    """ Returns an approximation of the number of given objects.
        Two objects are described as being "a pair",
        smaller than eight is "several",
        smaller than twenty is "a number of",
        smaller than two hundred are "dozens",
        anything bigger is described as being tens or hundreds of thousands or millions.
        For example: approximate("chicken", 100) => "dozens of chickens".
    """
    try: p = pluralize(word, custom=plural)
    except:
        raise TypeError, "can't pluralize %s, only str and unicode" % word.__class__.__name__
    # Anything up to 200.
    if amount == 0: 
        return "%s %s" % (NONE, p)
    if amount == 1: 
        return referenced(word) # "a" chicken, "an" elephant
    if amount == 2: 
        return "%s %s" % (PAIR, p)
    if 3 <= amount < 8: 
        return "%s %s" % (SEVERAL, p)
    if 8 <= amount < 18: 
        return "%s %s" % (NUMBER, p)
    if 18 <= amount < 23: 
        return "%s %s" % (SCORE, p)
    if 23 <= amount < 200: 
        return "%s %s" % (DOZENS, p)
    if amount > 10000000:
        return "%s %s" % (COUNTLESS, p)
    # Hundreds and thousands.
    thousands = int(log(amount, 10) / 3)
    hundreds  = ceil(log(amount, 10) % 3) - 1
    h = hundreds==2 and "hundreds of " or (hundreds==1 and "tens of " or "")
    t = thousands>0 and pluralize(ORDER[thousands])+" of " or ""
    return "%s%s%s" % (h, t, p)
        
#print approximate("chicken", 0)
#print approximate("chicken", 1)
#print approximate("chicken", 2)
#print approximate("chicken", 3)
#print approximate("chicken", 10)
#print approximate("chicken", 100)
#print approximate("chicken", 1000)
#print approximate("chicken", 10000)
#print approximate("chicken", 100000)
#print approximate("chicken", 1000000)
#print approximate("chicken", 10000000)
#print approximate("chicken", 100000000)
#print approximate("chicken", 10000000000)

#--- COUNT -----------------------------------------------------------------------------------------

# count(word, amount, plural={})
# count([word1, word2, ...], plural={})
# counr({word1:0, word2:0, ...}, plural={})
def count(*args, **kwargs):
    """ Returns an approximation of the entire set.
        Identical words are grouped and counted and then quantified with an approximation.
    """
    if len(args) == 2 and isinstance(args[0], basestring):
        return approximate(args[0], args[1], kwargs.get("plural", {}))
    if len(args) == 1 and isinstance(args[0], basestring) and "amount" in kwargs:
        return approximate(args[0], kwargs["amount"], kwargs.get("plural", {}))
    if len(args) == 1 and isinstance(args[0], dict):
        count = args[0]
    if len(args) == 1 and isinstance(args[0], (list, tuple)):
        # Keep a count of each item in the list.
        count = {}
        for word in args[0]:
            try:
                count.setdefault(word, 0)
                count[word] += 1
            except:
                raise TypeError, "can't count %s, only str and unicode" % word.__class__.__name__
    # Create an iterator of (count, item) tuples, sorted highest-first.
    s = [(count[word], word) for word in count]
    s = max([n for (n,w) in s]) > 1 and reversed(sorted(s)) or s
    # Concatenate approximate quantities of each item,
    # starting with the one that has the highest occurence.
    phrase = []
    for i, (n, word) in enumerate(s):
        phrase.append(approximate(word, n, kwargs.get("plural", {})))
        phrase.append(i==len(count)-2 and " and " or ", ")
    return "".join(phrase[:-1])

quantify = count
    
#print count(["goose", "goose", "duck", "chicken", "chicken", "chicken"])
#print count(["penguin", "polar bear"])
#print count(["whale"])

#--- REFLECT ---------------------------------------------------------------------------------------

readable_types = (
    ("^<type '"        , ""),
    ("^<class '(.*)'\>", "\\1 class"),
    ("'>"              , ""),
    ("pyobjc"          , "PyObjC"),
    ("objc_class"      , "Objective-C class"),
    ("objc"            , "Objective-C"),
    ("<objective-c class  (.*) at [0-9][0-9|a-z]*>" , "Objective-C \\1 class"),
    ("bool"            , "boolean"),
    ("int"             , "integer"),
    ("long"            , "long integer"),
    ("float"           , "float"),
    ("str"             , "string"),
    ("unicode"         , "unicode string"),
    ("dict"            , "dictionary"),
    ("NoneType"        , "None type"),
    ("instancemethod"  , "instance method"),
    ("builtin_function_or_method" , "built-in function"),
    ("classobj"        , "class object"),
    ("\."              , " "),
    ("_"               , " ")        
)

def reflect(object, quantify=True, replace=readable_types):
    """ Returns the type of each object in the given object.
        - For modules, this means classes and functions etc.
        - For list and tuples, means the type of each item in it.
        - For other objects, means the type of the object itself.
    """
    _type = lambda object: type(object).__name__
    types = []
    # Classes and modules with a __dict__ attribute listing methods, functions etc.  
    if hasattr(object, "__dict__"):
        # Function and method objects.
        if _type(object) in ("function", "instancemethod"):
            types.append(_type(object))
        # Classes and modules.
        else:
            for v in object.__dict__.values():
                try: types.append(str(v.__classname__))
                except:
                    # Not a class after all (some stuff like ufunc in Numeric).
                    types.append(_type(v))
    # Lists and tuples can consist of several types of objects.
    elif isinstance(object, (list, tuple, set)):
        types += [_type(x) for x in object]
    # Dictionaries have keys pointing to objects.
    elif isinstance(object, dict):
        types += [_type(k) for k in object]
        types += [_type(v) for v in object.values()]
    else:
        types.append(_type(object))
    # Clean up type strings.
    m = {}
    for i in range(len(types)):
        k = types[i]
        # Execute the regular expressions once only,
        # next time we'll have the conversion cached.
        if k not in m:
            for a,b in replace:
                types[i] = re.sub(a, b, types[i])      
            m[k] = types[i]      
        types[i] = m[k]
    if not quantify:
        if not isinstance(object, (list, tuple, set, dict)) and not hasattr(object, "__dict__"):
            return types[0]
        return types
    return count(types, plural={"built-in function" : "built-in functions"})

#print reflect("hello")
#print reflect(["hello", "goobye"])
#print reflect((1,2,3,4,5))
#print reflect({"name": "linguistics", "version": 1.0})
#print reflect(reflect)
#print reflect(__dict__)
#import Foundation; print reflect(Foundation)
#import Numeric; print reflect(Numeric)