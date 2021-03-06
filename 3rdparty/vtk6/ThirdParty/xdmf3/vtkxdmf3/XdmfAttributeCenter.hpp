/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfAttributeCenter.hpp                                             */
/*                                                                           */
/*  Author:                                                                  */
/*     Kenneth Leiter                                                        */
/*     kenneth.leiter@arl.army.mil                                           */
/*     US Army Research Laboratory                                           */
/*     Aberdeen Proving Ground, MD                                           */
/*                                                                           */
/*     Copyright @ 2011 US Army Research Laboratory                          */
/*     All Rights Reserved                                                   */
/*     See Copyright.txt for details                                         */
/*                                                                           */
/*     This software is distributed WITHOUT ANY WARRANTY; without            */
/*     even the implied warranty of MERCHANTABILITY or FITNESS               */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice             */
/*     for more information.                                                 */
/*                                                                           */
/*****************************************************************************/

#ifndef XDMFATTRIBUTECENTER_HPP_
#define XDMFATTRIBUTECENTER_HPP_

// Includes
#include "Xdmf.hpp"
#include "XdmfItemProperty.hpp"

/**
 * @brief Property describing where XdmfAttribute values are centered.
 *
 * XdmfAttributeCenter is a property used by XdmfAttribute to specify
 * where its values are centered on an XdmfGrid. A specific
 * XdmfAttributeCenter can be created by calling on of the static
 * methods in the class, i.e.  XdmfAttributeCenter::Cell().
 * Xdmf supports the following attribute centers:
 *
 * Example of use:
 *
 * C++
 *
 * @dontinclude ExampleXdmfAttribute.cpp
 * @skipline //#initialization
 * @until //#initialization
 * @skipline //#setCenter
 * @until //#setCenter
 * @skipline //#getCenter
 * @until //#getCenter
 *
 * Python
 *
 * @dontinclude XdmfExampleAttribute.py
 * @skipline #//initialization
 * @until #//initialization
 * @skipline #//setCenter
 * @until #//setCenter
 * @skipline #//getCenter
 * @until #//getCenter
 *
 *   Grid
 *   Cell
 *   Face
 *   Edge
 *   Node
 */
class XDMF_EXPORT XdmfAttributeCenter : public XdmfItemProperty {

public:

  virtual ~XdmfAttributeCenter();

  friend class XdmfAttribute;

  // Supported Xdmf Attribute Centers
  static shared_ptr<const XdmfAttributeCenter> Grid();
  static shared_ptr<const XdmfAttributeCenter> Cell();
  static shared_ptr<const XdmfAttributeCenter> Face();
  static shared_ptr<const XdmfAttributeCenter> Edge();
  static shared_ptr<const XdmfAttributeCenter> Node();

  void
  getProperties(std::map<std::string, std::string> & collectedProperties) const;


protected:

  /**
   * Protected constructor for XdmfAttributeCenter.  The constructor
   * is protected because all attribute centers supported by Xdmf
   * should be accessed through more specific static methods that
   * construct XdmfAttributeCenters -
   * i.e. XdmfAttributeCenter::Node().
   *
   * @param     name    The name of the XdmfAttributeCenter to construct.
   */
  XdmfAttributeCenter(const std::string & name);

private:

  XdmfAttributeCenter(const XdmfAttributeCenter &); // Not implemented.
  void operator=(const XdmfAttributeCenter &); // Not implemented.

  static shared_ptr<const XdmfAttributeCenter>
  New(const std::map<std::string, std::string> & itemProperties);

  std::string mName;
};

#endif /* XDMFATTRIBUTECENTER_HPP_ */
