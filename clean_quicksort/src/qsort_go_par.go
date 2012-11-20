package main

import (
    "fmt"
    "math/rand"
    "os"
    "strconv"
    "time"
)

const spawnThreshold = 500

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


func partition(tab []int) (t1 []int, t2 []int) {

    var pivot = tab[0]
    var lastLEPivot = len(tab) - 1
    var firstGTPivot = 1

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
            swap_in_tab(tab, 0, lastLEPivot)
            check(firstGTPivot == lastLEPivot + 1, "firstGTPivot != lastLEPivot + 1")
            break
        }
    }
    
    return tab[:lastLEPivot], tab[lastLEPivot+1:]
}



func quicksort(tab []int) {
    fakeChan := make(chan bool)
    _quicksort(tab, fakeChan, false)
}



func _quicksort(tab []int, done chan bool, inCoroutine bool) {
    if len(tab) > 1 {
        childrenDone := make(chan bool) /* To wait for the return */
        t1, t2 := partition(tab)

        if len(tab) > spawnThreshold {
            go _quicksort(t1, childrenDone, true)
            go _quicksort(t2, childrenDone, true)

            /* wait for both children */
            <-childrenDone
            <-childrenDone
        } else {
            /* Array too small, don't spawn the machinery */
            _quicksort(t1, childrenDone, false)
            _quicksort(t2, childrenDone, false)
        }
    }

    if inCoroutine {
        done<- true
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

    if len(os.Args) < 2 {
        fmt.Println("Usage: qsort_go.go nItems")
        os.Exit(1)
    }

    nItems, err := strconv.Atoi(os.Args[1])
    if err != nil {
        fmt.Printf("Failed to parse '%s' as int\n", os.Args[1])
        os.Exit(1)
    }

    arr := makeArray(nItems)

    startTime := time.Now()
    quicksort(arr)
    duration := time.Since(startTime)

    if (checkSorted(arr)) {
        fmt.Printf("{\"nElems\": %d, \"time\": %f}\n", nItems, duration.Seconds());
    } else {
        fmt.Println("ERROR: the array is not sorted")
    }
}
