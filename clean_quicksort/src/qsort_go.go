package main

import (
    "fmt"
    "math/rand"
)

const spawnThreshold = 100
const nbItems = 1000000

func check(predicate bool, msg string) {
    if !predicate {
        fmt.Printf("Assertion failed: %s\n", msg);
    }
}


func swap_in_tab(tab []int, i int, j int) {
    tmp := tab[i]
    tab[i] = tab[j]
    tab[j] = tmp
}


func makeArray(nbItems int) []int {
    arr := make([]int, nbItems)

    for i := 0; i < len(arr); i++ {
        arr[i] = rand.Int() % len(arr)
    }

    return arr
}



/* Return pivot and partition IN PLACE the array */
func partition(tab []int, p int, r int) (pivotIndex int) {

    var pivot = tab[p]
    var lastLEPivot = r
    var firstGTPivot = p + 1

    for {
        /* Find the last element that is <= pivot, checking we do not go too far */
        for lastLEPivot >= firstGTPivot && tab[lastLEPivot] > pivot {
            lastLEPivot--
        }

        /* Find the first element that is > pivot, checking we do not go too far */
        for firstGTPivot <= lastLEPivot && tab[firstGTPivot] <= pivot {
            firstGTPivot++
        }

        /* Do we have to swap ? */
        if firstGTPivot < lastLEPivot {
            swap_in_tab(tab, firstGTPivot, lastLEPivot)
        } else {
            /* Our array is partitioned, put the pivot in the right place */
            swap_in_tab(tab, p, lastLEPivot)
            check(firstGTPivot == lastLEPivot + 1, "firstGTPivot != lastLEPivot + 1")
            return lastLEPivot
        }
    }
    
    return -1 /* Bad, but don't care for this small test */
}



func quicksort(tab []int, p int, r int) {
    if p < r {
        q := partition(tab, p, r)

        quicksort(tab, p, q-1)
        quicksort(tab, q+1, r)
    }
}



func checkSorted(tab []int) bool {
    for i:= 1; i < len(tab); i++ {
        if tab[i] < tab[i-1] {
            return false
        }
    }

    return true
}


func main() {
    arr := makeArray(20)
    quicksort(arr, 0, len(arr) - 1)

    if (checkSorted(arr)) {
        fmt.Println("Gagné !")
    } else {
        fmt.Println("Perdu !")
    }
}
