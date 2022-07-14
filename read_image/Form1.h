#pragma once
#include <atlstr.h>
#include <iostream>
#include "image.h"


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
	private: System::Windows::Forms::PictureBox^ pictureBox2;
	private: System::Windows::Forms::PictureBox^ pictureBox3;
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
		void ShowGrayImages(image im) {
			pictureBox2->Width = im.w;
			pictureBox2->Height = im.h;
			pictureBox2->Refresh();
			Bitmap^ surface = gcnew Bitmap(im.w, im.h);
			pictureBox2->Image = surface;
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
		void ShowSmoothImages(image im) {
			pictureBox3->Width = im.w;
			pictureBox3->Height = im.h;
			pictureBox3->Refresh();
			Bitmap^ surface = gcnew Bitmap(im.w, im.h);
			pictureBox3->Image = surface;
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
		void InitializeComponent(void)
		{
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->BeginInit();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1808, 28);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->openToolStripMenuItem });
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(46, 24);
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
			this->pictureBox1->Location = System::Drawing::Point(653, 604);
			this->pictureBox1->Margin = System::Windows::Forms::Padding(4);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(521, 514);
			this->pictureBox1->TabIndex = 1;
			this->pictureBox1->TabStop = false;
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// pictureBox2
			// 
			this->pictureBox2->Location = System::Drawing::Point(670, 31);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(519, 514);
			this->pictureBox2->TabIndex = 2;
			this->pictureBox2->TabStop = false;
			// 
			// pictureBox3
			// 
			this->pictureBox3->Location = System::Drawing::Point(1285, 31);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(523, 514);
			this->pictureBox3->TabIndex = 3;
			this->pictureBox3->TabStop = false;
			// 
			// pictureBox4
			// 
			this->pictureBox4->Location = System::Drawing::Point(34, 47);
			this->pictureBox4->Name = L"pictureBox4";
			this->pictureBox4->Size = System::Drawing::Size(492, 435);
			this->pictureBox4->TabIndex = 4;
			this->pictureBox4->TabStop = false;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1808, 977);
			this->Controls->Add(this->pictureBox4);
			this->Controls->Add(this->pictureBox3);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->EndInit();
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
			 //ShowRGBImages(im);
			 image gray_im = Rgb_to_gray(im);
			 //ShowGrayImages(gray_im);
			 image smooth = Smoothing(gray_im);
			 ShowGrayImages(smooth);
			 //ShowSmoothImages(smooth);

			 double* angles = new double[smooth.w * smooth.h];
			 double* EdgeImage = Gradient(smooth, angles);
			 image test = make_image(smooth.w, smooth.h, 1);
			 for (int i = 0; i < test.w*test.h; i++){
				 test.data[i] = EdgeImage[i];// Testing
			 }
			 ShowSmoothImages(test);
			 image BinaryEdgeImage = CannyEdge(EdgeImage, smooth.w, smooth.h, angles);
			 ShowEdgeImages(BinaryEdgeImage);
			 int HoughWidth, HoughHeight;
			 int* HoughSpaceLine = HoughLine(BinaryEdgeImage, BinaryEdgeImage.w, BinaryEdgeImage.h, angles, HoughWidth,  HoughHeight);
			 //ShowEdgeImages(angl);

			 std::cout <<"w: " << im.w<<"\n";
			 std::cout <<"c: " << im.c << "\n";
			 std::cout << "data " << (int)im.data[10];
		}//
		
	}//openTool
};
}
