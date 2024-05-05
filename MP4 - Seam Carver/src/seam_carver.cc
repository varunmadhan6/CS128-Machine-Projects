#include "seam_carver.hpp"

// implement the rest of SeamCarver's functions here

// given functions below, DO NOT MODIFY

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}

const ImagePPM& SeamCarver::GetImage() const {
  return image_;
}

int SeamCarver::GetHeight() const {
  return height_;
}

int SeamCarver::GetWidth() const {
  return width_;
}

int SeamCarver::GetEnergy(int row, int col) const {
  int row_up = row - 1;
  if (row_up < 0) {
    row_up = height_ - 1;
  }
  int row_down = row + 1;
  if (row_down == height_) {
    row_down = 0;
  }
  int col_left = col - 1;
  if (col_left < 0) {
    col_left = width_ - 1;
  }
  int col_right = col + 1;
  if (col_right == width_) {
    col_right = 0;
  }
  Pixel up = image_.GetPixel(row_up, col);
  Pixel down = image_.GetPixel(row_down, col);
  Pixel left = image_.GetPixel(row, col_left);
  Pixel right = image_.GetPixel(row, col_right);
  int col_energy = (up.GetRed() - down.GetRed())*(up.GetRed() - down.GetRed()) + (up.GetBlue() - down.GetBlue())*(up.GetBlue() - down.GetBlue()) 
  + (up.GetGreen() - down.GetGreen())*(up.GetGreen() - down.GetGreen());
  int row_energy = (left.GetRed() - right.GetRed())*(left.GetRed() - right.GetRed()) + (left.GetBlue() - right.GetBlue())*(left.GetBlue() - right.GetBlue()) 
  + (left.GetGreen() - right.GetGreen())*(left.GetGreen() - right.GetGreen());
  return row_energy + col_energy;
}
int* SeamCarver::GetHorizontalSeam() const {
  return FindMinPathDynamicProgrammingHorz(height_, width_);
}
int* SeamCarver::GetVerticalSeam() const {
  return FindMinPathDynamicProgrammingVert(height_, width_);
}
void SeamCarver::RemoveHorizontalSeam() {
  image_.RemoveHorizontal(GetHorizontalSeam());
  height_ = image_.GetHeight();
}
void SeamCarver::RemoveVerticalSeam() {
  image_.RemoveVertical(GetVerticalSeam());
  width_ = image_.GetWidth();
}
int SeamCarver::Min(int idx_one, int idx_two, int row_, int** two_dim_array) const {
  int val_one = two_dim_array[row_][idx_one];
  int val_two = two_dim_array[row_][idx_two];
  if (val_two < val_one) {
    return idx_two;
  } else {
    return idx_one;
  }
}
int SeamCarver::Min(int idx_one, int idx_two, int idx_three, int row_, int** two_dim_array) const {
  int val_one = two_dim_array[row_][idx_one];
  int val_two = two_dim_array[row_][idx_two];
  int val_three = two_dim_array[row_][idx_three];
  if (val_three < val_two && val_three < val_one) {
    return idx_three;
  } else if (val_one < val_two && val_one <= val_three) {
    return idx_one;
  } else {
    return idx_two;
  }
}
int* SeamCarver::FindMinPathDynamicProgrammingHorz(int row_size, int col_size) const {
  int** values = new int*[col_size];
  for (int col_tmp = 0; col_tmp < col_size; ++col_tmp) {
    values[col_tmp] = new int[row_size];
    if (col_tmp == col_size - 1) {
      for (int row_tmp = 0; row_tmp < row_size; ++row_tmp) {
        values[col_tmp][row_tmp] = GetEnergy(row_tmp, col_tmp);
      }
    }
  }
  for (int col_tmp = col_size - 2; col_tmp >= 0; --col_tmp) {
    for (int row_tmp = 0; row_tmp < row_size; ++row_tmp) {
      int best = 0;
      if (row_tmp == 0) {
        best = Min(row_tmp, row_tmp + 1, col_tmp + 1, values);
      } else if (row_tmp == row_size - 1) {
        best = Min(row_tmp - 1, row_tmp, col_tmp + 1, values);
      } else {
        best = Min(row_tmp - 1,row_tmp, row_tmp + 1, col_tmp + 1, values);
      }
      values[col_tmp][row_tmp] = values[col_tmp + 1][best] + GetEnergy(row_tmp, col_tmp);
    }
  }
  return Trace(col_size, row_size, values);
}
int* SeamCarver::FindMinPathDynamicProgrammingVert(int row_size, int col_size) const {
  int** values = new int*[row_size];
  for (int row_tmp = 0; row_tmp < row_size; ++row_tmp) {
    values[row_tmp] = new int[col_size];
    if (row_tmp == row_size - 1) {
      for (int col_tmp = 0; col_tmp < col_size; ++col_tmp) {
        values[row_tmp][col_tmp] = GetEnergy(row_tmp, col_tmp);
      }
    }
  }
  for (int row_tmp = row_size - 2; row_tmp >= 0; --row_tmp) {
    for (int col_tmp = 0; col_tmp < col_size; ++col_tmp) {
      int best = 0;
      if (col_tmp == 0) {
        best = Min(col_tmp, col_tmp + 1, row_tmp + 1, values);
      } else if (col_tmp == col_size - 1) {
        best = Min(col_tmp - 1, col_tmp, row_tmp + 1, values);
      } else {
        best = Min(col_tmp - 1, col_tmp, col_tmp + 1, row_tmp + 1, values);
      }
      values[row_tmp][col_tmp] = values[row_tmp + 1][best] + GetEnergy(row_tmp, col_tmp);
    }
  }
  return Trace(row_size, col_size, values);
}
int* SeamCarver::Trace(int row_size, int col_size, int** arr_two_dim) const {
  int* real_arr = new int[row_size];
  int minimum = arr_two_dim[0][0];
  int idx = 0;
  for (int col_tmp = 0; col_tmp < col_size; ++col_tmp) {
      if (arr_two_dim[0][col_tmp] < minimum) {
        minimum = arr_two_dim[0][col_tmp];
        idx = col_tmp;
      }
  }
  real_arr[0] = idx;
  for (int row_tmp = 1; row_tmp < row_size; ++row_tmp) {
    if (idx == 0) {
      idx = Min(idx, idx + 1, row_tmp, arr_two_dim);
    } else if (idx == col_size - 1) {
      idx = Min(idx - 1, idx, row_tmp, arr_two_dim);
    } else {
      idx = Min(idx - 1, idx, idx + 1, row_tmp, arr_two_dim);
    }
    real_arr[row_tmp] = idx;
  }
  for (int row = 0; row < row_size; ++row) {
    delete[] arr_two_dim[row];
  }
  delete[] arr_two_dim;
  arr_two_dim = nullptr;
  return real_arr;
}