#pragma once

#include <ImageOptions>
#include <QtCore/QBitArray>

class SupportedImageOptions
{
public:
    inline SupportedImageOptions() = default;
    inline SupportedImageOptions(const SupportedImageOptions &other) = default;
    inline SupportedImageOptions(SupportedImageOptions &&other) = default;
    inline SupportedImageOptions(std::initializer_list<ImageOptions::Option> options);
    inline ~SupportedImageOptions() = default;

    inline SupportedImageOptions &operator= (const SupportedImageOptions &other) = default;
    inline SupportedImageOptions &operator= (SupportedImageOptions &&other) = default;

    inline bool contains(ImageOptions::Option option) const
    {
        return _data.at(option);
    }

    inline void insert(ImageOptions::Option option)
    {
        _data[option] = true;
    }

    inline void remove(ImageOptions::Option option)
    {
        _data[option] = false;
    }

private:
    QBitArray _data {ImageOptions::OptionsCount, false};
};

inline SupportedImageOptions::SupportedImageOptions(std::initializer_list<ImageOptions::Option> options)
{
    for (auto option : options) {
        _data[option] = true;
    }
}
