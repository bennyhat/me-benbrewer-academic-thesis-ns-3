/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Benjamin Brewer
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Benjamin Brewer <bb1267@my.bristol.ac.uk>
 */
#ifndef CLICK_NODE_CONTAINER_H
#define CLICK_NODE_CONTAINER_H

#include "ns3/node-container.h"

namespace ns3 {

class ClickNodeContainer : public NodeContainer
{
public:
  ClickNodeContainer ();

  virtual void Create (uint32_t n);
  virtual void Create (uint32_t n, uint32_t systemId);
};

} // namespace ns3

#endif /* NODE_CONTAINER_H */
