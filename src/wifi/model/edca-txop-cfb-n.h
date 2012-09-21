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
#ifndef EDCA_TXOP_CFB_N_H
#define EDCA_TXOP_CFB_N_H

#include "edca-txop-n.h"
#include "mac-low.h"

namespace ns3 {

class EdcaTxopCfbN : public EdcaTxopN
{
public:
	static TypeId GetTypeId (void);
	EdcaTxopCfbN ();
	virtual ~EdcaTxopCfbN ();

	virtual void GotAck (double snr, WifiMode txMode);
	virtual void MissedAck (void);
	virtual void NotifyAccessGranted (void);
	virtual void NotifyCollision (void);
	virtual void GotCts (double snr, WifiMode txMode);
	virtual void MissedCts (void);
	virtual void Queue (Ptr<const Packet> packet, const WifiMacHeader &hdr);

protected:
	virtual void ContinueCfbIfNeeded (MacLowTransmissionParameters &a_params);
	virtual void ContinueCfb ();
	virtual void CheckCfb (void);
	virtual void ResetCfb (void);
	virtual void DoStart (void);
	virtual bool IsBurstable (WifiMacHeader hdr, Ptr<const Packet> pkt);

	virtual Ptr<const Packet> GetNextPacket (WifiMacHeader &hdr, MacLowTransmissionParameters &params);
	virtual MacLowTransmissionParameters DetermineParameters (WifiMacHeader hdr, Ptr<const Packet> pkt);

	Time m_cfbElapsed;
	Time m_cfbStarted;
	Time m_currentDuration;
	MacLowTransmissionParameters m_currentParams;
	uint32_t  m_defaultAifs;
};
}  // namespace ns3

#endif /* EDCA_TXOP_CFB_N_H */
