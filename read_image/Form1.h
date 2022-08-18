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
	private: System::Windows::Forms::ToolStripMenuItem^ edgeDetectionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ circleDetectionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ gradientCalculationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ gradientMagnitudeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ gradientAngleToolStripMenuItem;




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
		void ShowDetectedImages(image im) {
			pictureBox4->Width = im.w;
			pictureBox4->Height = im.h;
			pictureBox4->Refresh();
			Bitmap^ surface = gcnew Bitmap(im.w, im.h);
			pictureBox4->Image = surface;
			Color c;
			int psw, bufpos;
			psw = im.w * 3;
			for (int row = 0; row < im.h; row++)
				for (int col = 0; col < im.w; col++) {
					bufpos = row * psw + col * im.c;
					c = Color::FromArgb(im.data[bufpos], im.data[bufpos + 1], im.data[bufpos + 2]);
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
		
		//void DrawSelectedCircles(int* HoughSpace, int HoughWidth, int HoughHeight, image im, int radius, int number_of_circles, int radius_scale) {
		//	int* max = new int[number_of_circles];
		//	int* a_axis = new int[number_of_circles];//x
		//	int* b_axis = new int[number_of_circles];//y
		//	int* r_axis = new int[number_of_circles];
		//	int r_min = radius - radius_scale;
		//	int r_max = radius + radius_scale;
		//	for (int i = 0; i < number_of_circles; i++){
		//		max[i] = 0;
		//		a_axis[i] = 0;
		//		b_axis[i] = 0;
		//		r_axis[i] = 0;
		//	}
		//	//HoughSpace[(row * HoughWidth + col) * (r - r_min) + r - r_min] < 1806840 &&
		//	for (int row = 1; row < HoughHeight-1; row++)
		//		for (int col = 1; col < HoughWidth-1; col++) 
		//			for (int r = r_min; r < r_max; r++)
		//				if (HoughSpace[(row * HoughWidth + col) * (r - r_min) + r - r_min] < 15000 && HoughSpace[(row * HoughWidth + col)*(r-r_min)+r-r_min] > max[number_of_circles - 1]) {
		//					for (int i = 1; i < number_of_circles; i++) {// 
		//						max[i - 1] = max[i];
		//						a_axis[i - 1] = a_axis[i];
		//						b_axis[i - 1] = b_axis[i];
		//						r_axis[i - 1] = r_axis[i];
		//					}
		//					max[number_of_circles - 1] = HoughSpace[(row * HoughWidth + col) * (r - r_min) + r - r_min];// max vote
		//					a_axis[number_of_circles - 1] = col;// x axsis
		//					b_axis[number_of_circles - 1] = row;// y axis
		//					r_axis[number_of_circles - 1] = r;// r axis
		//				}
		//	for (int i = 0; i < number_of_circles; i++){
		//		std::cout << "-"<<i<<"-Max vote  " << max[i] << "\n";
		//		std::cout << "-"<<i<<"-A value " << a_axis[i]<< "\n";
		//		std::cout << "-" << i << "-B value" << b_axis[i] << "\n";
		//		std::cout << "-"<<i<<"-R value  " << r_axis[i] << "\n---------------------------------------------------\n";
		//	}
		//	int psw, bufpos;
		//	psw = im.w * 3;
		//	for (int i = 0; i < number_of_circles; i++)
		//		for (int theta = 0; theta < 360; theta++){

		//			/*int col = a_axis[i] + r_axis[i] * cos(theta * PI / 180);
		//			int row = b_axis[i] + r_axis[i] * sin(theta * PI / 180);
		//			if ((col >= 1 && col < Width - 1) && (row >= 1 && row < Height - 1)) {
		//				bufpos = row * psw + col * im.c;
		//				im.data[bufpos] = 255, im.data[bufpos + 1] = 0, im.data[bufpos + 2] = 0;
		//			}*/
		//			int col = a_axis[i] - r_axis[i] * cos(theta * PI / 180);
		//			int row = b_axis[i] - r_axis[i] * sin(theta * PI / 180);
		//			if ((col >= 1 && col < Width - 1) && (row >= 1 && row < Height - 1)) {
		//				bufpos = row * psw + col * im.c;
		//				im.data[bufpos] = 255, im.data[bufpos + 1] = 0, im.data[bufpos + 2] = 0;
		//			}
		//		}
		//	ShowDetectedImages(im);
		//}
		void InitializeComponent(void)
		{
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->edgeDetectionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->circleDetectionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->gradientCalculationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->gradientMagnitudeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->gradientAngleToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
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
			this->menuStrip1->Size = System::Drawing::Size(1808, 28);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->openToolStripMenuItem,
					this->edgeDetectionToolStripMenuItem, this->circleDetectionToolStripMenuItem, this->gradientCalculationToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(86, 24);
			this->fileToolStripMenuItem->Text = L"Open File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(227, 26);
			this->openToolStripMenuItem->Text = L"Line Detection";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem_Click);
			// 
			// edgeDetectionToolStripMenuItem
			// 
			this->edgeDetectionToolStripMenuItem->Name = L"edgeDetectionToolStripMenuItem";
			this->edgeDetectionToolStripMenuItem->Size = System::Drawing::Size(227, 26);
			this->edgeDetectionToolStripMenuItem->Text = L"Edge Detection";
			this->edgeDetectionToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::edgeDetectionToolStripMenuItem_Click);
			// 
			// circleDetectionToolStripMenuItem
			// 
			this->circleDetectionToolStripMenuItem->Name = L"circleDetectionToolStripMenuItem";
			this->circleDetectionToolStripMenuItem->Size = System::Drawing::Size(227, 26);
			this->circleDetectionToolStripMenuItem->Text = L"Circle Detection";
			this->circleDetectionToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::circleDetectionToolStripMenuItem_Click);
			// 
			// gradientCalculationToolStripMenuItem
			// 
			this->gradientCalculationToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->gradientMagnitudeToolStripMenuItem,
					this->gradientAngleToolStripMenuItem
			});
			this->gradientCalculationToolStripMenuItem->Name = L"gradientCalculationToolStripMenuItem";
			this->gradientCalculationToolStripMenuItem->Size = System::Drawing::Size(227, 26);
			this->gradientCalculationToolStripMenuItem->Text = L"Gradient Calculation";
			// 
			// gradientMagnitudeToolStripMenuItem
			// 
			this->gradientMagnitudeToolStripMenuItem->Name = L"gradientMagnitudeToolStripMenuItem";
			this->gradientMagnitudeToolStripMenuItem->Size = System::Drawing::Size(225, 26);
			this->gradientMagnitudeToolStripMenuItem->Text = L"Gradient Magnitude";
			this->gradientMagnitudeToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::gradientMagnitudeToolStripMenuItem_Click);
			// 
			// gradientAngleToolStripMenuItem
			// 
			this->gradientAngleToolStripMenuItem->Name = L"gradientAngleToolStripMenuItem";
			this->gradientAngleToolStripMenuItem->Size = System::Drawing::Size(225, 26);
			this->gradientAngleToolStripMenuItem->Text = L"Gradient Angle";
			this->gradientAngleToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::gradientAngleToolStripMenuItem_Click);
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
			this->pictureBox4->Location = System::Drawing::Point(962, 31);
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

			 str = openFileDialog1->FileName;
			 CStringA s2(str);
			 const char* input = s2;
			 image im = load_image(input);
			 ShowRGBImages(im);
			 image GrayImage = RgbToGray(im);

			 image SmoothImage = Smoothing(GrayImage, 5);

			 gradient grad;

			 grad = Gradient(SmoothImage, grad); // gradient calculation

			 image BinaryEdgeImage = CannyEdge(grad);// Edge detection
			 //ShowEdgeImages(BinaryEdgeImage);

			 hough HoughSpaceLine = HoughLine(BinaryEdgeImage, HoughSpaceLine);

			 int number_of_lines=10;
			 DrawSelectedLines( HoughSpaceLine, im , number_of_lines);// Rgb line image
			 ShowDetectedImages(im);

		}//
		
	}//openTool
	private: System::Void circleDetectionToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		CString str;
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {

			str = openFileDialog1->FileName;
			CStringA s2(str);
			const char* input = s2;
			image im = load_image(input);
			ShowRGBImages(im);
			image GrayImage = RgbToGray(im);

			image SmoothImage = Smoothing(GrayImage, 5);

			gradient grad;

			grad = Gradient(SmoothImage, grad); // gradient calculation

			image BinaryEdgeImage = CannyEdge(grad);// Edge detection

			int radius = 50;
			int radius_scale = 35;
			int number_of_circles=10;
			
			hough HoughSpaceCircle = HoughCircle(HoughSpaceCircle, BinaryEdgeImage, grad, radius,radius_scale);
						
			DrawSelectedCircles(HoughSpaceCircle, im, radius, number_of_circles,radius_scale);
			ShowDetectedImages(im);
		}
	}
	private: System::Void edgeDetectionToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		CString str;
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			str = openFileDialog1->FileName;
			CStringA s2(str);
			const char* input = s2;
			image im = load_image(input);
			ShowRGBImages(im);
			image GrayImage = RgbToGray(im);

			image SmoothImage = Smoothing(GrayImage,3);

			gradient grad = Gradient(SmoothImage, grad); // gradient magnitude array (gx^2+gy^2)
			
			image BinaryEdgeImage = CannyEdge(grad);// Edge detection
			ShowEdgeImages(BinaryEdgeImage);
		}
	}
	private: System::Void gradientMagnitudeToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		CString str;
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			str = openFileDialog1->FileName;

			CStringA s2(str);
			const char* input = s2;

			image im = load_image(input);
			ShowRGBImages(im);

			image GrayImage = RgbToGray(im);
			image SmoothImage = Smoothing(GrayImage,5);

			gradient grad = Gradient(SmoothImage, grad);
			image GradientImage = make_image(grad.w, grad.h, 1);

			for (int i = 0; i < grad.w*grad.h; i++)
				GradientImage.data[i] = grad.magnititude[i];

			ShowEdgeImages(GradientImage);
		}
	}
	private: System::Void gradientAngleToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		CString str;
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {

			str = openFileDialog1->FileName;
			CStringA s2(str);
			const char* input = s2;

			image im = load_image(input);
			ShowRGBImages(im);

			image GrayImage = RgbToGray(im);
			image SmoothImage = Smoothing(GrayImage, 5);

			gradient grad = Gradient(SmoothImage, grad);
			image GradientImage = make_image(grad.w, grad.h, 1);

			for (int i = 0; i < grad.w * grad.h; i++)
				GradientImage.data[i] = grad.direction[i];

			ShowEdgeImages(GradientImage);
		}
	}
};
}
