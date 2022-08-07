package main

import (
	"fmt"
	"log"
	"bufio"
	"os"
	"os/exec"
	"io"
	"io/ioutil"
	"net/http"
	"github.com/go-echarts/go-echarts/v2/charts"
	"github.com/go-echarts/go-echarts/v2/types"
	"github.com/go-echarts/go-echarts/v2/opts"
	"strings"
	"strconv"
	"encoding/csv"
)

var mainlog *log.Logger
var errlog *log.Logger

var reloadNeeded string

var highArr []float64
var widthArr []float64
var highLen int

var header [8]string

func sayhello(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Привет!")
}

func errorHandler(err error) {
	mainlog.Println("Error detected, look at error.log")
	errlog.Fatal(err)
}

func initLogging() {
	var mainlogfile *os.File
	var errlogfile *os.File
	var err error
	mainlogfile, err = os.OpenFile("log.txt", os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0666)
	if (err != nil) {
		log.Fatal(err)
	}
	errlogfile, err = os.OpenFile("log.txt", os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0666)
	if (err != nil) {
		log.Fatal(err)
	}
	mainlog = log.New(mainlogfile, "MAIN: | ", log.Ldate|log.Ltime)
	errlog = log.New(errlogfile, "ERROR: | ", log.Ldate|log.Ltime)
}

func serveFiles(w http.ResponseWriter, r *http.Request) {
	for cnt := 1; cnt != highLen; cnt++ {
		renderChart(highArr, cnt, widthArr, cnt)
	}
	fmt.Println(r.URL.Path, "path")
	p := "." + r.URL.Path
	http.ServeFile(w, r, p)
}

func LinesFromReader(r io.Reader) ([]string, error) {
	var lines []string
	scanner := bufio.NewScanner(r)
	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}
	if err := scanner.Err(); err != nil {
		return nil, err
	}

	return lines, nil
}

func File2lines(filePath string) ([]string, error) {
	f, err := os.Open(filePath)
	if err != nil {
		return nil, err
	}
	defer f.Close()
	return LinesFromReader(f)
}

func InsertStringToFile(path, str string, index int) error {
	lines, err := File2lines(path)
	if err != nil {
		return err
	}
	fileContent := ""
	for i, line := range lines {
		if i == index {
			fileContent += str
		}
		fileContent += line
		fileContent += "\n"
	}
	return ioutil.WriteFile(path, []byte(fileContent), 0644)
}

func generateLineItems(a []float64, s int) []opts.LineData {
	items := make([]opts.LineData, 0)
	for i := 0; i < s; i++ {
		items = append(items, opts.LineData{Value: a[i]})
	}
	return items
}

func renderChart(a []float64, alen int, b []float64, blen int) {
	line := charts.NewLine()
	line.SetGlobalOptions(
		charts.WithInitializationOpts(opts.Initialization{
			Theme: types.ThemeInfographic,
		}),
		charts.WithTitleOpts(opts.Title{
			Title:    "Грофек",
			Subtitle: "ужасъ",
		}),
	)
	var lenHarr []int
	for i := 0; i < alen; i++ {
		lenHarr = append(lenHarr, i)
	}
	line.SetXAxis(lenHarr).
	AddSeries("Category A", generateLineItems(a, alen)).
	AddSeries("Category B", generateLineItems(b, blen)).
	SetSeriesOptions(charts.WithLineChartOpts(opts.LineChart{Smooth: true}))
	f, _ := os.Create("line.html")
	_ = line.Render(f)
	reloadNeeded = "1"
}


func main() {
	header[0] = "<!DOCTYPE html>\n"
	header[1] = "<html>\n"
	header[2] = "<head>\n"
	header[3] = "    <meta charset=\"utf-8\">\n"
	header[4] = "    <title>Awesome go-echarts</title>\n"
	header[5] = "		 <script src=\"echarts.min.js\"></script>\n"
	header[6] = "    <script src=\"infographic.js\"></script>\n"
	header[7] = "		 <script>var startxhttp = new XMLHttpRequest();startxhttp.open(\"GET\", \"/upddone\", true);startxhttp.send();setInterval(function() {var xhttp = new XMLHttpRequest();xhttp.onreadystatechange = function() {if (this.status == 200) {if(Number(this.responseText)==1){console.log(\"will reload\");window.location.reload();}}};xhttp.open(\"GET\", \"/updneeded\", true);xhttp.send();},300);</script>\n"
	out, error := exec.Command("./remove.bash").Output()
	fmt.Println(out, error, "asd")
	for i := 0; i < 8; i++ {
		err := InsertStringToFile("line1.html", header[i], i)
		if err != nil {
			errorHandler(err)
		}
	}
	http.HandleFunc("/", serveFiles) // Устанавливаем роутер
	http.HandleFunc("/updneeded", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprintf(w, reloadNeeded)
		if (reloadNeeded == "1") {
			reloadNeeded = "0"
		} else {
			reloadNeeded = "1"
		}
	})
	file, ferr := os.Open("hw_200.csv")
	if (ferr != nil) {
		log.Fatal(ferr)
	}
	reader := csv.NewReader(file)
	reader.Comma = ','
	high := make(map [int]float64)
	width := make(map [int]float64)
	num := 0
	widthLen := 0
	for {
		result, rerr := reader.Read()
		if (rerr == io.EOF) {
			break
		}
		for id, val := range result {
			val = strings.TrimSpace(val)
			fmt.Println(id, val, "dsa")
			if (id == 0) {
				num, _ = strconv.Atoi(val)
			}
			if (id == 1) {
				high[num], _ = strconv.ParseFloat(val, 64)
				highLen++
			}
			if (id == 2) {
				width[num], _ = strconv.ParseFloat(val, 64)
				widthLen++
			}
		}
	}
	fmt.Println(high, width, "sad")
	for i := 0; i < highLen; i++ {
		highArr = append(highArr, high[i])
	}
	for i := 0; i < widthLen; i++ {
		widthArr = append(widthArr, width[i])
	}
	err := http.ListenAndServe(":8080", nil) // устанавливаем порт веб-сервера
	if err != nil {
		log.Fatal("ListenAndServe: ", err)
	}
	//readfile, err  = os.OpenFile("line0.html",  os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0666)
}
