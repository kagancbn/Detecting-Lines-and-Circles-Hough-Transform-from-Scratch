#pragma once
#include <atlstr.h>
#include <iostream>
#include "image.h"
#define PI 3.141592

namespace read_image {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	protected:
	private: System::Windows::Forms::ToolStripMenuItem^ fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ openToolStripMenuItem;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;


	private: System::Windows::Forms::PictureBox^ pictureBox4;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void ShowRGBImages(image im) {  
			pictureBox1->Width = im.w;
			pictureBox1->Height = im.h;
			pictureBox1->Refresh();
			Bitmap^ surface = gcnew Bitmap(im.w, im.h);
			pictureBox1->Image = surface;
			Color c;
			int psw, bufpos;
			psw = im.w * 3;
			for (int row = 0; row < im.h; row++)
				for (int col = 0; col < im.w; col++){
					bufpos = row * psw + col * im.c;
					c = Color::FromArgb(im.data[bufpos], im.data[bufpos+1], im.data[bufpos+2]);
					surface->SetPixel(col, row, c);
				}
		}//ShowImages
		void ShowEdgeImages(image im) {
			pictureBox4->Width = im.w;
			pictureBox4->Height = im.h;
			pictureBox4->Refresh();
			Bitmap^ surface = gcnew Bitmap(im.w, im.h);
			pictureBox4->Image = surface;
			Color c;
			int psw, bufpos;
			psw = im.w;
			for (int row = 0; row < im.h; row++)
				for (int col = 0; col < im.w; col++) {
					bufpos = row * psw + col;
					c = Color::FromArgb(im.data[bufpos], im.data[bufpos], im.data[bufpos]);
					surface->SetPixel(col, row, c);
				}
		}//ShowImages
		void DrawSelectedLines(int* HoughSpace, int HoughWidth, int HoughHeight, image im, int number_of_lines) {

			int* max = new int[number_of_lines];
			int* distance = new int [number_of_lines];
			int* theta = new int[number_of_lines];
			for (int i = 0; i < number_of_lines; i++){
				max[i] = 0, distance[i] = 0, theta[i] = 0;
			}

			for (int row = 0; row < HoughHeight; row++)// distance indice d
				for (int col = 0; col < HoughWidth; col++){//Theta indice 
					if (HoughSpace[row * HoughWidth + col] > max[number_of_lines-1]) {
						for (int i = 1; i < number_of_lines; i++){// 
							max[i - 1] = max[i];
							distance[i - 1] = distance[i];
							theta[i - 1] = theta[i];
						}
						max[number_of_lines-1] = HoughSpace[row * HoughWidth + col];// max vote
						distance[number_of_lines-1] = row;// perpendecular distance value of max vote
						theta[number_of_lines-1] = col;// theta value of max vote
					}
				}

			for (int i = 0; i < number_of_lines; i++){
				std::cout << "-"<<i<<"-Max vote = " << max[i] << "\n";
				std::cout << "-"<<i<<"-Max distance = " << distance[i]<< "\n";
				std::cout << "-"<<i<<"-Max theta = " << theta[i] << "\n---------------------------------------------------\n";
			}
			int psw, bufpos;
			psw = im.w * 3;
			for (int i = 0; i < number_of_lines; i++)
				for (int row = 1; row < im.h-1; row++)
					for (int col = 1; col < im.w-1; col++) {
						if (distance[i] == int (col * cos(theta[i] * PI / 180) + row * sin(theta[i] * PI / 180) + HoughHeight / 2 )) {
							bufpos = row * psw + col * im.c;
							im.data[bufpos]= 255 , im.data[bufpos + 1]= 0, im.data[bufpos + 2]=0;
						}
					}
			ShowRGBImages(im);
		}
		void InitializeComponent(void)
		{
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->BeginInit();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1808, 30);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->openToolStripMenuItem });
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(46, 26);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(128, 26);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(26, 43);
			this->pictureBox1->Margin = System::Windows::Forms::Padding(4);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(378, 376);
			this->pictureBox1->TabIndex = 1;
			this->pictureBox1->TabStop = false;
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// pictureBox4
			// 
			this->pictureBox4->Location = System::Drawing::Point(760, 43);
			this->pictureBox4->Name = L"pictureBox4";
			this->pictureBox4->Size = System::Drawing::Size(468, 376);
			this->pictureBox4->TabIndex = 4;
			this->pictureBox4->TabStop = false;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1808, 977);
			this->Controls->Add(this->pictureBox4);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void openToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		CString str; 
		
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			 //pictureBox1->ImageLocation = openFileDialog1->FileName;
			 str = openFileDialog1->FileName;
			 CStringA s2(str);
			 const char* input = s2;

			 image im = load_image(input);
			 image gray_im = Rgb_to_gray(im);
			 image smooth = Smoothing(gray_im);

			 double* angles = new double[smooth.w * smooth.h];
			 double* EdgeImage = Gradient(smooth, angles);

			 image BinaryEdgeImage = CannyEdge(EdgeImage, smooth.w, smooth.h, angles);
			 ShowEdgeImages(BinaryEdgeImage);// canny edge image
			 int HoughWidth, HoughHeight;
			 int* HoughSpaceLine = HoughLine(BinaryEdgeImage, BinaryEdgeImage.w, BinaryEdgeImage.h, angles, HoughWidth,  HoughHeight);
			 int number_of_lines=10;
			 DrawSelectedLines( HoughSpaceLine, HoughWidth, HoughHeight, im , number_of_lines);// Rgb line image

		}//
		
	}//openTool
};
}
