for i in {1..100}
do
    python gen2.py 500 > teste$i.in
    ./proj2 < teste$i.in > teste$i.out
done