#include "grayscale_image.hpp"

GrayscaleImage::GrayscaleImage(const ElevationDataset& dataset) {
    width_ = dataset.Width();
    height_ = dataset.Height();
    image_ = std::vector<std::vector<Color>>(height_, std::vector<Color>(width_));
    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            int shade_of_gray = 0;
            if (dataset.MaxEle() != dataset.MinEle()) {
                double ratio = static_cast<double>(dataset.DatumAt(i,j) - dataset.MinEle()) / (dataset.MaxEle() - dataset.MinEle());
                shade_of_gray = std::round(ratio * kMaxColorValue);
            }
            Color flavor(shade_of_gray, shade_of_gray, shade_of_gray);
            image_[i][j] = flavor;
        }
    }
}

GrayscaleImage::GrayscaleImage(const std::string& filename, size_t width, size_t height) {
    ElevationDataset elevation(filename, width, height);
    width_ = width;
    height_ = height;
    image_ = std::vector<std::vector<Color>>(height_, std::vector<Color>(width_));
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            int shade_of_gray = 0;
            if (elevation.MaxEle() != elevation.MinEle()) {
                double ratio = static_cast<double>(elevation.DatumAt(i,j) - elevation.MinEle()) / (elevation.MaxEle() - elevation.MinEle());
                shade_of_gray = std::round(ratio * kMaxColorValue);
            }
            Color flavor(shade_of_gray, shade_of_gray, shade_of_gray);
            image_[i][j] = flavor;
        }
    }
}

size_t GrayscaleImage::Width() const {
    return width_;
}

size_t GrayscaleImage::Height() const {
    return height_;
}

unsigned int GrayscaleImage::MaxColorValue() const {
    return kMaxColorValue;
}

const Color& GrayscaleImage::ColorAt(int row, int col) const {
    return image_.at(row).at(col);
}

const std::vector<std::vector<Color> >& GrayscaleImage::GetImage() const {
    return image_;
}

void GrayscaleImage::ToPpm( const std::string& name ) const {
    std::ofstream ofs {name};
    ofs << "P3" << "\n";
    ofs << width_ << " " << height_ << "\n";
    ofs << kMaxColorValue << "\n";
    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            Color c = image_.at(i).at(j);
            ofs << c.Red() << " " << c.Green() << " " << c.Blue() << " ";
        }
        ofs << "\n";
    }
    ofs << std::endl;
}