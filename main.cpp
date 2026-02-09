#include <iostream>
#include "image.h"
#include "image_processor.h"
#include "sorting.h"
#include "utils.h"
#include <string>

int main() {
    cout<<"Moguci algoritmi"<<endl;
    cout<<"Unsite 1 ako zelite ExampleSort rekostruise sliku"<<endl;
    cout<<"Unesite 2 ako zelite MatijaSort rekostruise sliku"<<endl;
    cout<<"Unestie 3 ako zelite AnastasijaSort rekostruise sliku"<<endl;
    cout<<"Unestite 4 ako zelite da vam se izpisu sva vremena za sve slike"<<endl;
    cout<<"Unesite Algoritam koji zelite da koristite"<<endl;
    int n;
    cin>>n;
    if (n==1) {
        SortingAlgorithm* sorting = new ExampleSort();
        ImageProcessor::reconstructImage("/Users/ivanristovic/CLionProjects/untitled/datoteke/input.txt","/Users/ivanristovic/CLionProjects/untitled/datoteke/output.txt",sorting);
        delete sorting;
    }
    else if (n==2) {
        int k;
        cout<<"Uneiste broj k po kome se vrsi algoritam: ";
        cin>>k;
        SortingAlgorithm* sorting=new MatijaSort(k);
        ImageProcessor::reconstructImage("/Users/ivanristovic/CLionProjects/untitled/datoteke/image_2_rand.txt","/Users/ivanristovic/CLionProjects/untitled/datoteke/output.txt",sorting);
        delete sorting;
    }
    else if (n==3) {
        int k;
        double c;
        cout<<"Unesite konstatnu c: "; cin>>c;
        cout<<"Unesite broj k: "; cin>>k;
        SortingAlgorithm* sorting=new AnastasijaSort(k,c);
        ImageProcessor::reconstructImage("/Users/ivanristovic/CLionProjects/untitled/datoteke/image_2_rand.txt","/Users/ivanristovic/CLionProjects/untitled/datoteke/output.txt",sorting);
        delete sorting;
    }
    else if (n==4) {
        Stopwatch* stopwatch=new Stopwatch();
        string outputFile="/Users/ivanristovic/CLionProjects/untitled/datoteke/output.txt";
        string inputFiles[12]=
            {
            "/Users/ivanristovic/CLionProjects/untitled/datoteke/image_1_asc.txt",
            "/Users/ivanristovic/CLionProjects/untitled/datoteke/image_1_desc.txt",
            "/Users/ivanristovic/CLionProjects/untitled/datoteke/image_1_rand.txt",
            "/Users/ivanristovic/CLionProjects/untitled/datoteke/image_2_asc.txt",
            "/Users/ivanristovic/CLionProjects/untitled/datoteke/image_2_desc.txt",
            "/Users/ivanristovic/CLionProjects/untitled/datoteke/image_2_rand.txt",
            "/Users/ivanristovic/CLionProjects/untitled/datoteke/image_3_asc.txt",
            "/Users/ivanristovic/CLionProjects/untitled/datoteke/image_3_desc.txt",
            "/Users/ivanristovic/CLionProjects/untitled/datoteke/image_3_rand.txt",
            "/Users/ivanristovic/CLionProjects/untitled/datoteke/image_4_asc.txt",
            "/Users/ivanristovic/CLionProjects/untitled/datoteke/image_4_desc.txt",
            "/Users/ivanristovic/CLionProjects/untitled/datoteke/image_4_rand.txt"
            };
        SortingAlgorithm* sort1=new ExampleSort();
        SortingAlgorithm* sort2=new CppSort();
        SortingAlgorithm* sort3=new MatijaSort(100);
        SortingAlgorithm* sort4=new AnastasijaSort(2,0.2);
        SortingAlgorithm* sorts[4]={sort1,sort2,sort3,sort4};
        cout<<"Tabela: "<<endl;
        cout<<"-----------------------------------------------------------------------------------"<<endl;
        cout<<"Ime fajla      |ExampleSort      |CppSort         |MatijaSort      |AnastasijaSort   "<<endl;
        cout<<"-----------------------------------------------------------------------------------"<<endl;
        for(int i=0;i<12;i++) {
            cout << std::filesystem::path(inputFiles[i]).filename().string();
            for (int j = 0; j < 4; j++) {
                ifstream file(inputFiles[i]);
                int rows, cols;
                file >> rows >> cols;
                int pixelCount = rows * cols;
                Pixel* pixels = new Pixel[pixelCount];
                for (int k = 0; k < pixelCount; k++) {
                    string line;
                    file >> line;
                    size_t commaPos = line.find(',');
                    pixels[k].index = stoi(line.substr(0, commaPos));
                    pixels[k].value = line[commaPos + 1];
                }
                Image* image = new Image(pixels, rows, cols);
                stopwatch->start();
                sorts[j]->sort(image, ASCENDING);
                long long duration=stopwatch->stop();
                cout << " | " << duration << " ms";
                delete image;

            }
            cout << '\n';

        }
        delete sort1;
        delete sort2;
        delete sort3;
        delete sort4;
        delete stopwatch;
    }
}
