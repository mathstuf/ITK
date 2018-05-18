/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#include "itkHalfHermitianToRealInverseFFTImageFilter.h"

#ifndef itkFFTWHalfHermitianToRealInverseFFTImageFilter_h
#define itkFFTWHalfHermitianToRealInverseFFTImageFilter_h

#include "itkFFTWCommon.h"

namespace itk
{
/** \class FFTWHalfHermitianToRealInverseFFTImageFilter
 *
 * \brief FFTW-based reverse Fast Fourier Transform
 *
 * This filter computes the reverse Fourier transform of an image. The
 * implementation is based on the FFTW library.
 *
 * This filter is multithreaded and supports input images of any size.
 *
 * This implementation was taken from the Insight Journal paper:
 * https://hdl.handle.net/10380/3154
 * or http://insight-journal.com/browse/publication/717
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \ingroup FourierTransform
 * \ingroup MultiThreaded
 * \ingroup ITKFFT
 *
 * \sa FFTWGlobalConfiguration
 * \sa InverseFFTImageFilter
 */
template< typename TInputImage, typename TOutputImage=Image< typename TInputImage::PixelType::value_type, TInputImage::ImageDimension> >
class ITK_TEMPLATE_EXPORT FFTWHalfHermitianToRealInverseFFTImageFilter:
  public HalfHermitianToRealInverseFFTImageFilter< TInputImage, TOutputImage >
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(FFTWHalfHermitianToRealInverseFFTImageFilter);

  /** Standard class type aliases. */
  using InputImageType = TInputImage;
  using InputPixelType = typename InputImageType::PixelType;
  using InputSizeType = typename InputImageType::SizeType;
  using OutputImageType = TOutputImage;
  using OutputPixelType = typename OutputImageType::PixelType;
  using OutputRegionType = typename OutputImageType::RegionType;
  using OutputSizeType = typename OutputImageType::SizeType;

  using Self = FFTWHalfHermitianToRealInverseFFTImageFilter;
  using Superclass = HalfHermitianToRealInverseFFTImageFilter< InputImageType, OutputImageType >;
  using Pointer = SmartPointer< Self >;
  using ConstPointer = SmartPointer< const Self >;

  /** The proxy type is a wrapper for the FFTW API since the proxy is
   * only defined over double and float, trying to use any other pixel
   * type is unsupported, as is trying to use double if only the float
   * FFTW version is configured in, or float if only double is
   * configured. */
  using FFTWProxyType = typename fftw::Proxy< OutputPixelType >;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FFTWHalfHermitianToRealInverseFFTImageFilter,
               HalfHermitianToRealInverseFFTImageFilter);

  /** Define the image dimension. */
  static constexpr unsigned int ImageDimension = InputImageType::ImageDimension;

  /** Set/Get the behavior of wisdom plan creation. The default is
   * provided by FFTWGlobalConfiguration::GetPlanRigor().
   *
   * The parameter is one of the FFTW planner rigor flags FFTW_ESTIMATE, FFTW_MEASURE,
   * FFTW_PATIENT, FFTW_EXHAUSTIVE provided by FFTWGlobalConfiguration.
   * /sa FFTWGlobalConfiguration
   */
  virtual void SetPlanRigor( const int & value )
  {
    // Use that method to check the value.
    FFTWGlobalConfiguration::GetPlanRigorName( value );
    if( m_PlanRigor != value )
      {
      m_PlanRigor = value;
      this->Modified();
      }
  }
  itkGetConstReferenceMacro( PlanRigor, int );
  void SetPlanRigor( const std::string & name )
  {
    this->SetPlanRigor( FFTWGlobalConfiguration::GetPlanRigorValue( name ) );
  }

  SizeValueType GetSizeGreatestPrimeFactor() const override;

protected:
  FFTWHalfHermitianToRealInverseFFTImageFilter();
  ~FFTWHalfHermitianToRealInverseFFTImageFilter() override {}

  void UpdateOutputData(DataObject *output) override;

  void BeforeThreadedGenerateData() override;

  void DynamicThreadedGenerateData(const OutputRegionType& outputRegionForThread) override;

  void PrintSelf(std::ostream & os, Indent indent) const override;

private:
  bool m_CanUseDestructiveAlgorithm;

  int m_PlanRigor;

};


} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFFTWHalfHermitianToRealInverseFFTImageFilter.hxx"
#endif

#endif //itkFFTWHalfHermitianToRealInverseFFTImageFilter_h
