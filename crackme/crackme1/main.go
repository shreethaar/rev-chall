package main

import (
    "bufio"
    "fmt"
    "os"
)

func revStr(s string) string {
    runes:=[]rune(s) 
    for i,j:=0,len(runes)-1;i<j;i,j=i+1,j-1{
        runes[i],runes[j]=runes[j],runes[i] 
    }
    return string(runes) 
}

func main() {
    flag:="CTF{flag}"
    check:=revStr(flag);
    fmt.Println("Enter flag:")
    scanner:=bufio.NewScanner(os.Stdin)
    scanner.Scan()
    input:=scanner.Text()
    if input == check {
        fmt.Println("Correct Flag!",flag)
    } else {
        fmt.Println("Wrong Flag!")
    }
}

