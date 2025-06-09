 # * Onluktan (Decimal) Onaltılık (Hexadecimal) çevirme:

'''
sayi = int(input())
kalanlar = []
binary = 0

while sayi > 0:
   k = sayi % 16
   kalanlar.append(k)
   sayi = sayi // 16

if 10 in kalanlar:
    index = kalanlar.index(10)
    kalanlar[index] = "A"
if 11 in kalanlar:
    index = kalanlar.index(11)
    kalanlar[index] = "B"
if 12 in kalanlar:
    index = kalanlar.index(12)
    kalanlar[index] = "C"
if 13 in kalanlar:
    index = kalanlar.index(13)
    kalanlar[index] = "D"
if 14 in kalanlar:
    index = kalanlar.index(14)
    kalanlar[index] = "E"
if 15 in kalanlar:
    index = kalanlar.index(15)
    kalanlar[index] = "F"

binary = ''.join(map(str, kalanlar[::-1]))
print(binary)
'''

#* Armstrong sayı denetleme ( 157 -> 1³ + 5³ + 7³ = 157)

'''
sayi = int(input())
sayi2 = 0
def is_armstrong(n):
    basamak_sayisi = len(str(sayi))
    global sayi2
    for digit in str(sayi):
        sayi2 = sayi2 + (int(digit) ** basamak_sayisi)

    if sayi2 == sayi:
        return "True"
    else:
        return "False"

print(is_armstrong(sayi))
'''

#* Kombinasyon hesaplama
'''
import math

n = int(input())
r = int(input())
l = n-r

def C(n,r):
    return math.factorial(n) / (math.factorial(r) * math.factorial(l))

print(C(n,r))
'''

#* 0-n aralığındaki tüm Armstrong sayıları bulma

'''
n = int(input())
armstrongSayilar = []

def is_armstrong(a):
   stra = str(a)
   ussu = len(str(a))
   sayi = 0
   for digit in stra:
       sayi += int(digit) ** ussu
   if sayi == a:
       armstrongSayilar.append(a)        

for i in range(10,n):
   is_armstrong(i)

print(armstrongSayilar)
'''

#* Sayının her basamağının faktöriyelinin toplamı kendisine eşit mi?

'''
import math
n = int(input())
factorial = 0
faxw = []
for digit in str(n):
    factorial += math.factorial(int(digit))
    faxw.append(f'{digit}!')

if factorial == n:
    printmessage = ' + '.join(faxw)
    print(printmessage + f' = {n}')
else:
    print("nah verecem n")
'''

#* 16'lı sistemden (Hexadecimal), ikili sisteme (Binary) veri çevirme
'''
hex_to_bin_table = {
    '0': '0000', '1': '0001', '2': '0010', '3': '0011',
    '4': '0100', '5': '0101', '6': '0110', '7': '0111',
    '8': '1000', '9': '1001', 'A': '1010', 'B': '1011',
    'C': '1100', 'D': '1101', 'E': '1110', 'F': '1111'
}

hexadecimalInput = input("Hexadecimal girin => ").upper()
binary_result = "" 

for key in hexadecimalInput:
    if key in hex_to_bin_table:
        binary_result += hex_to_bin_table[key]
    else:
        print("Yanlış örnek")
        break;

print(binary_result)
'''

#* İkilik sistemden (binary) 10'luk sisteme (decimal) çevirme

'''
binaryInput = input("Binary girin => ")
decimalResult = 0

for index, digit in enumerate(binaryInput[::-1]):
    decimalResult += int(digit) * pow(2,index)

print(decimalResult)
'''

#* Itertools uygulamaları

##* Tüm olasılıkları üretmek
'''
from itertools import product

olasilikSayi = 0

for p in product(["K", "K", "K", "M", "M", "Y"], repeat=4):
    print(p)
    olasilikSayi+=1

print(olasilikSayi)
'''

##* Permütasyon bulmak
'''
from itertools import permutations

olasilikSayi = 0

for p in permutations("MAMA"):
    print(p)
    olasilikSayi+=1

print(olasilikSayi)
'''


##* Kombinasyon bulmak (aynı eleman tekrar edemez)
'''
from itertools import combinations

for c in combinations("MAMA", 3):
    print(c)
'''

##* Kombinasyon bulmak (aynı eleman tekrar edebilir)
'''
from itertools import combinations_with_replacement

for c in combinations_with_replacement("MAMA", 3):
    print(c)
'''

#! product() - Sıralı, tekrarlı
#! combinations() - Sırasız, tekrarsız
#! combinations_with_replacement() - Sırasız, tekrarlı
#! permutations() - Sıralı, tekrarsız

digits = input().split()
digits = list(map(int, digits))
digitsAvg = sum(digits) / len(digits)
AvgUstu = list(filter(lambda x: x>digitsAvg, digits))
print(','.join(list(map(str, AvgUstu))))