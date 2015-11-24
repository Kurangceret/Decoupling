dofile("Scripts/PlayerScript.lua")
dofile ("Scripts/ConstantScript.lua")


downAttackOrigin = {48, 67}
sideAttackOrigin = {56, 65}
upAttackOrigin = {40, 62}






Animations = {
  {
				name = "Standing_Up",
				textureLocation = TextureDir .. "human_back_attack.png",
				origLists = {
					{
						frame = {frameSizeHeavyAttackUp[1] * 0, frameSizeHeavyAttackUp[2] * 0, 0, 0},
						duration = 0.2
					},

				},
				wholeFrameSize = frameSizeHeavyAttackUp,
				wholeDuration = 0.6,
				flipX = false,
				endToStart = false,
				isRepeating = true,
        transformOrigin = upAttackOrigin

			},
			{
				name = "Standing_Down",
				textureLocation = TextureDir .. "human_front_attack.png",
				origLists = {
					{
						frame = {frameSizeHeavyAttackDown[1] * 0, frameSizeHeavyAttackDown[1] * 0, 0, 0},
						duration = 0.2
					},

				},
				wholeFrameSize = frameSizeHeavyAttackDown,
				wholeDuration = 0.6,
				flipX = false,
				endToStart = false,
				isRepeating = true,
        transformOrigin = downAttackOrigin

			},
			{
				name = "Standing_Left",
				textureLocation = TextureDir .. "human_side_attack.png",
				origLists = {
					{
						frame = {0, 0, 0, 0},
						duration = 0.2
					},

				},
				wholeFrameSize = {frameSizeMovSide[1], frameSizeMovSide[2]},
				wholeDuration = 0.6,
				flipX = true,
				endToStart = false,
				isRepeating = true,
        transformOrigin = {36.0, 54.0}

			},
			{
				name = "Standing_Right",
				textureLocation = TextureDir .. "human_side_attack.png",
				origLists = {
					{
						frame = {0, 0, 0, 0},
						duration = 0.2
					},

				},
				wholeFrameSize = {frameSizeMovSide[1], frameSizeMovSide[2]},
				wholeDuration = 0.6,
				flipX = false,
				endToStart = false,
				isRepeating = true,
        transformOrigin = {36.0, 54.0}

			},
			{
				name = "Moving_Up",
				textureLocation = TextureDir .. "human_back_attack.png",
				origLists = {
					{
						frame = {frameSizeHeavyAttackUp[1] * 1, frameSizeHeavyAttackUp[2] * 0, 0, 0},
						duration = 0.15
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 2, frameSizeHeavyAttackUp[2] * 0, 0, 0},
						duration = 0.15
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 3, frameSizeHeavyAttackUp[2] * 0, 0, 0},
						duration = 0.15
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 0, frameSizeHeavyAttackUp[2] * 1, 0, 0},
						duration = 0.15
					}
				},
				wholeFrameSize = frameSizeHeavyAttackUp,
				wholeDuration = 0.6,
				flipX = false,
				endToStart = true,
				isRepeating = true,
        transformOrigin = upAttackOrigin

			},

			{
				name = "Moving_Down",
				textureLocation = TextureDir .. "human_front_attack.png",
				
				origLists = {
					{
						frame = {frameSizeHeavyAttackDown[1] * 1, frameSizeHeavyAttackDown[2] * 0, 0, 0},
						duration = 0.15
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 2, frameSizeHeavyAttackDown[2] * 0, 0, 0},
						duration = 0.15
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 3, frameSizeHeavyAttackDown[2] * 0, 0, 0},
						duration = 0.15
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 0, frameSizeHeavyAttackDown[2] * 1, 0, 0},
						duration = 0.15
					}
				},
				wholeDuration = 0.6,
        wholeFrameSize = frameSizeHeavyAttackDown,
				flipX = false,
				endToStart = true,
				isRepeating = true,
        transformOrigin = downAttackOrigin
			},

			{
				name = "Moving_Left",
				textureLocation = TextureDir .. "human_side_attack.png",
				origLists = {
					{
						frame = {frameSizeMovSide[1] * 1, 0, 0, 0},
						duration = 0.15
					},
					{
						frame = {frameSizeMovSide[1] * 2, 0, 0, 0},
						duration = 0.15
					},
					{
						frame = {frameSizeMovSide[1] * 3, 0, 0, 0},
						duration = 0.15
					},
					{
						frame = {frameSizeMovSide[1] * 4, 0, 0, 0},
						duration = 0.15
					}
				},
				wholeFrameSize = {frameSizeMovSide[1], frameSizeMovSide[2]},
				wholeDuration = 0.6,
				flipX = true,
				endToStart = true,
				isRepeating = true,
        transformOrigin = {36.0, 54.0}
        
			},

			{
				name = "Moving_Right",
				textureLocation = TextureDir .. "human_side_attack.png",
				origLists = {
					{
						frame = {frameSizeMovSide[1] * 1, 0, 0, 0},
						duration = 0.15
					},
					{
						frame = {frameSizeMovSide[1] * 2, 0, 0, 0},
						duration = 0.15
					},
					{
						frame = {frameSizeMovSide[1] * 3, 0, 0, 0},
						duration = 0.15
					},
					{
						frame = {frameSizeMovSide[1] * 4, 0, 0, 0},
						duration = 0.15
					}
				},
				wholeFrameSize = {frameSizeMovSide[1], frameSizeMovSide[2]},
				wholeDuration = 0.6,
				flipX = false,
				endToStart = true,
				isRepeating = true,
        transformOrigin = {36.0, 54.0}
			},
      
     
      
      {
        name = "Heavy_Attack_Right_1",
				textureLocation = TextureDir .. "human_side_swing_sword.png",
				origLists = {
          {
						frame = {frameSizeHeavyAttackSide[1] * 0, frameSizeHeavyAttackSide[2] * 0, 0, 0},
						duration = animPrepAttackHeavy1Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 1, frameSizeHeavyAttackSide[2] * 0, 0, 0},
						duration = animPrepAttackHeavy1Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 2, frameSizeHeavyAttackSide[2] * 0, 0, 0},
						duration = animPrepAttackHeavy1Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 3, frameSizeHeavyAttackSide[2] * 0, 0, 0},
						duration = animPrepAttackHeavy1Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 0, frameSizeHeavyAttackSide[2] * 1, 0, 0},
						duration = animAttackHeavy1Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 1, frameSizeHeavyAttackSide[2] * 1, 0, 0},
						duration = animAttackHeavy1Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 2, frameSizeHeavyAttackSide[2] * 1, 0, 0},
						duration = animAttackHeavy1Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 3, frameSizeHeavyAttackSide[2] * 1, 0, 0},
						duration = animAttackHeavy1Dur
					},
          {
						frame = {frameSizeHeavyAttackSide[1] * 0, frameSizeHeavyAttackSide[2] * 2, 0, 0},
						duration = animAttackHeavy1Dur
					}
				},
				wholeFrameSize = {frameSizeHeavyAttackSide[1], frameSizeHeavyAttackSide[2]},
				wholeDuration = 0.6,
				flipX = false,
				endToStart = false,
				isRepeating = false,
        transformOrigin = {56.0, 65.0} 
        
      },
      
      
      
      {
        name = "Heavy_Attack_Right_2",
				textureLocation = TextureDir .. "human_side_swing_sword.png",
				origLists = {
          {
						frame = {frameSizeHeavyAttackSide[1] * 1, frameSizeHeavyAttackSide[2] * 2, 0, 0},
						duration = animPrepAttackHeavy2Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 2, frameSizeHeavyAttackSide[2] * 2, 0, 0},
						duration = animPrepAttackHeavy2Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 3, frameSizeHeavyAttackSide[2] * 2, 0, 0},
            duration = animPrepAttackHeavy2Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 0, frameSizeHeavyAttackSide[2] * 3, 0, 0},
						duration = animPrepAttackHeavy2Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 1, frameSizeHeavyAttackSide[2] * 3, 0, 0},
						duration = animAttackHeavy2Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 2, frameSizeHeavyAttackSide[2] * 3, 0, 0},
						duration = animAttackHeavy2Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 3, frameSizeHeavyAttackSide[2] * 3, 0, 0},
						duration = animAttackHeavy2Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 0, frameSizeHeavyAttackSide[2] * 4, 0, 0},
						duration = animAttackHeavy2Dur
					},
          {
						frame = {frameSizeHeavyAttackSide[1] * 1, frameSizeHeavyAttackSide[2] * 4, 0, 0},
						duration = animAttackHeavy2Dur
					}
				},
				wholeFrameSize = {frameSizeHeavyAttackSide[1], frameSizeHeavyAttackSide[2]},
				wholeDuration = 0.6,
				flipX = false,
				endToStart = false,
				isRepeating = false,
        transformOrigin = {56.0, 65.0} 
        
      },
            
      
      {
        name = "Heavy_Attack_Right_3",
				textureLocation = TextureDir .. "human_side_swing_sword.png",
				origLists = {
          {
						frame = {frameSizeHeavyAttackSide[1] * 2, frameSizeHeavyAttackSide[2] * 4, 0, 0},
						duration = animPrepAttackHeavy3Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 3, frameSizeHeavyAttackSide[2] * 4, 0, 0},
						duration = animPrepAttackHeavy3Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 0, frameSizeHeavyAttackSide[2] * 5, 0, 0},
						duration = animPrepAttackHeavy3Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 1, frameSizeHeavyAttackSide[2] * 5, 0, 0},
						duration = animPrepAttackHeavy3Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 2, frameSizeHeavyAttackSide[2] * 5, 0, 0},
						duration = animAttackHeavy3Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 3, frameSizeHeavyAttackSide[2] * 5, 0, 0},
						duration = animAttackHeavy3Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 0, frameSizeHeavyAttackSide[2] * 6, 0, 0},
						duration = animAttackHeavy3Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 1, frameSizeHeavyAttackSide[2] * 6, 0, 0},
						duration = animAttackHeavy3Dur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 2, frameSizeHeavyAttackSide[2] * 6, 0, 0},
						duration = animAttackHeavy3Dur
					},
          {
						frame = {frameSizeHeavyAttackSide[1] * 3, frameSizeHeavyAttackSide[2] * 6, 0, 0},
						duration = animAttackHeavy3Dur
					}
				},
				wholeFrameSize = {frameSizeHeavyAttackSide[1], frameSizeHeavyAttackSide[2]},
				wholeDuration = 0.6,
				flipX = false,
				endToStart = false,
				isRepeating = false,
        transformOrigin = {56.0, 65.0} 
        
      },
      
      {
        name = "Recovery_Heavy_Attack_Right",
				textureLocation = TextureDir .. "human_side_swing_sword.png",
				origLists = {
          {
						frame = {frameSizeHeavyAttackSide[1] * 0, frameSizeHeavyAttackSide[2] * 7, 0, 0},
						duration = animRecoveryDur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 1, frameSizeHeavyAttackSide[2] * 7, 0, 0},
						duration = animRecoveryDur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 2, frameSizeHeavyAttackSide[2] * 7, 0, 0},
						duration = animRecoveryDur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 3, frameSizeHeavyAttackSide[2] * 7, 0, 0},
						duration = animRecoveryDur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 0, frameSizeHeavyAttackSide[2] * 8, 0, 0},
						duration = animRecoveryDur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 1, frameSizeHeavyAttackSide[2] * 8, 0, 0},
						duration = animRecoveryDur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 2, frameSizeHeavyAttackSide[2] * 8, 0, 0},
						duration = animRecoveryDur
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 3, frameSizeHeavyAttackSide[2] * 8, 0, 0},
						duration = animRecoveryDur
					}
				},
				wholeFrameSize = {frameSizeHeavyAttackSide[1], frameSizeHeavyAttackSide[2]},
				wholeDuration = 0.6,
				flipX = false,
				endToStart = false,
				isRepeating = false,
        transformOrigin = {56.0, 65.0} 
        
      },
      
      --------------------------------
      
      
      {
        name = "Heavy_Attack_Left_1",
				textureLocation = TextureDir .. "human_side_swing_sword.png",
				origLists = {
          {
						frame = {frameSizeHeavyAttackSide[1] * 0, frameSizeHeavyAttackSide[2] * 0, 0, 0},
						duration = prepAttackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 1, frameSizeHeavyAttackSide[2] * 0, 0, 0},
						duration = prepAttackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 2, frameSizeHeavyAttackSide[2] * 0, 0, 0},
						duration = prepAttackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 3, frameSizeHeavyAttackSide[2] * 0, 0, 0},
						duration = prepAttackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 0, frameSizeHeavyAttackSide[2] * 1, 0, 0},
						duration = attackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 1, frameSizeHeavyAttackSide[2] * 1, 0, 0},
            duration = attackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 2, frameSizeHeavyAttackSide[2] * 1, 0, 0},
						duration = attackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 3, frameSizeHeavyAttackSide[2] * 1, 0, 0},
						duration = attackHeavy1Dur / 4
					}
				},
				wholeFrameSize = {frameSizeHeavyAttackSide[1], frameSizeHeavyAttackSide[2]},
				wholeDuration = 0.6,
				flipX = true,
				endToStart = false,
				isRepeating = false,
        transformOrigin = {56.0, 65.0} 
        
      },
      
      
      
      {
        name = "Heavy_Attack_Left_2",
				textureLocation = TextureDir .. "human_side_swing_sword.png",
				origLists = {
          {
						frame = {frameSizeHeavyAttackSide[1] * 0, frameSizeHeavyAttackSide[2] * 2, 0, 0},
						duration = prepAttackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 1, frameSizeHeavyAttackSide[2] * 2, 0, 0},
						duration = prepAttackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 2, frameSizeHeavyAttackSide[2] * 2, 0, 0},
						duration = prepAttackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 3, frameSizeHeavyAttackSide[2] * 2, 0, 0},
						duration = prepAttackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 0, frameSizeHeavyAttackSide[2] * 3, 0, 0},
						duration = attackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 1, frameSizeHeavyAttackSide[2] * 3, 0, 0},
						duration = attackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 2, frameSizeHeavyAttackSide[2] * 3, 0, 0},
						duration = attackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 3, frameSizeHeavyAttackSide[2] * 3, 0, 0},
						duration = attackHeavy2Dur / 4
					}
				},
				wholeFrameSize = {frameSizeHeavyAttackSide[1], frameSizeHeavyAttackSide[2]},
				wholeDuration = 0.6,
				flipX = true,
				endToStart = false,
				isRepeating = false,
        transformOrigin = {56.0, 65.0} 
        
      },
      
      
      {
        name = "Heavy_Attack_Left_3",
				textureLocation = TextureDir .. "human_side_swing_sword.png",
				origLists = {
          {
						frame = {frameSizeHeavyAttackSide[1] * 0, frameSizeHeavyAttackSide[2] * 4, 0, 0},
						duration = prepAttackHeavy3Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 1, frameSizeHeavyAttackSide[2] * 4, 0, 0},
						duration = prepAttackHeavy3Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 2, frameSizeHeavyAttackSide[2] * 4, 0, 0},
						duration = prepAttackHeavy3Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 3, frameSizeHeavyAttackSide[2] * 4, 0, 0},
						duration = prepAttackHeavy3Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 0, frameSizeHeavyAttackSide[2] * 5, 0, 0},
						duration = attackHeavy3Dur / 5
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 1, frameSizeHeavyAttackSide[2] * 5, 0, 0},
						duration = attackHeavy3Dur / 5
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 2, frameSizeHeavyAttackSide[2] * 5, 0, 0},
						duration = attackHeavy3Dur / 5
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 3, frameSizeHeavyAttackSide[2] * 5, 0, 0},
						duration = attackHeavy3Dur / 5
					},
					{
						frame = {frameSizeHeavyAttackSide[1] * 0, frameSizeHeavyAttackSide[2] * 6, 0, 0},
						duration = attackHeavy3Dur / 5
					}
				},
				wholeFrameSize = {frameSizeHeavyAttackSide[1], frameSizeHeavyAttackSide[2]},
				wholeDuration = 0.6,
				flipX = true,
				endToStart = false,
				isRepeating = false,
        transformOrigin = {56.0, 65.0} 
        
      },
      ---------------
     
      {
				name = "Heavy_Attack_Down_1",
				textureLocation = TextureDir .. "human_front_attack.png",
				
				origLists = {
          {
						frame = {frameSizeHeavyAttackDown[1] * 1, frameSizeHeavyAttackDown[2] * 1, 0, 0},
						duration = prepAttackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 2, frameSizeHeavyAttackDown[2] * 1, 0, 0},
						duration = prepAttackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 3, frameSizeHeavyAttackDown[2] * 1, 0, 0},
						duration = prepAttackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 0, frameSizeHeavyAttackDown[2] * 2, 0, 0},
						duration = prepAttackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 1, frameSizeHeavyAttackDown[2] * 2, 0, 0},
						duration = attackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 2, frameSizeHeavyAttackDown[2] * 2, 0, 0},
						duration = attackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 3, frameSizeHeavyAttackDown[2] * 2, 0, 0},
						duration = attackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 0, frameSizeHeavyAttackDown[2] * 3, 0, 0},
						duration = attackHeavy1Dur / 4
					}
				},
				wholeDuration = 0.6,
        wholeFrameSize = frameSizeHeavyAttackDown,
				flipX = false,
				endToStart = false,
				isRepeating = false,
        transformOrigin = downAttackOrigin
			},
      
      {
				name = "Heavy_Attack_Down_2",
				textureLocation = TextureDir .. "human_front_attack.png",
				
				origLists = {
          {
						frame = {frameSizeHeavyAttackDown[1] * 1, frameSizeHeavyAttackDown[2] * 3, 0, 0},
						duration = prepAttackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 2, frameSizeHeavyAttackDown[2] * 3, 0, 0},
						duration = prepAttackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 3, frameSizeHeavyAttackDown[2] * 3, 0, 0},
						duration = prepAttackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 0, frameSizeHeavyAttackDown[2] * 4, 0, 0},
						duration = prepAttackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 1, frameSizeHeavyAttackDown[2] * 4, 0, 0},
						duration = attackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 2, frameSizeHeavyAttackDown[2] * 4, 0, 0},
						duration = attackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 3, frameSizeHeavyAttackDown[2] * 4, 0, 0},
						duration = attackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 0, frameSizeHeavyAttackDown[2] * 5, 0, 0},
						duration = attackHeavy2Dur / 4
					}
				},
				wholeDuration = 0.6,
        wholeFrameSize = frameSizeHeavyAttackDown,
				flipX = false,
				endToStart = false,
				isRepeating = false,
        transformOrigin = downAttackOrigin
			},
     
      {
				name = "Heavy_Attack_Down_3",
				textureLocation = TextureDir .. "human_front_attack.png",
				
				origLists = {
          {
						frame = {frameSizeHeavyAttackDown[1] * 1, frameSizeHeavyAttackDown[2] * 5, 0, 0},
						duration = prepAttackHeavy3Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 2, frameSizeHeavyAttackDown[2] * 5, 0, 0},
						duration = prepAttackHeavy3Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 3, frameSizeHeavyAttackDown[2] * 5, 0, 0},
						duration = prepAttackHeavy3Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 0, frameSizeHeavyAttackDown[2] * 6, 0, 0},
						duration = prepAttackHeavy3Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 1, frameSizeHeavyAttackDown[2] * 6, 0, 0},
						duration = attackHeavy3Dur / 5
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 2, frameSizeHeavyAttackDown[2] * 6, 0, 0},
						duration = attackHeavy3Dur / 5
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 3, frameSizeHeavyAttackDown[2] * 6, 0, 0},
						duration = attackHeavy3Dur / 5
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 0, frameSizeHeavyAttackDown[2] * 7, 0, 0},
						duration = attackHeavy3Dur / 5
					},
					{
						frame = {frameSizeHeavyAttackDown[1] * 1, frameSizeHeavyAttackDown[2] * 7, 0, 0},
						duration = attackHeavy3Dur / 5
					}
				},
				wholeDuration = 0.6,
        wholeFrameSize = frameSizeHeavyAttackDown,
				flipX = false,
				endToStart = false,
				isRepeating = false,
        transformOrigin = downAttackOrigin
			},
    {
				name = "Heavy_Attack_Up_1",
				textureLocation = TextureDir .. "human_back_attack.png",
				origLists = {
          {
						frame = {frameSizeHeavyAttackUp[1] * 1, frameSizeHeavyAttackUp[2] * 1, 0, 0},
						duration = prepAttackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 2, frameSizeHeavyAttackUp[2] * 1, 0, 0},
						duration = prepAttackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 3, frameSizeHeavyAttackUp[2] * 1, 0, 0},
						duration = prepAttackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 0, frameSizeHeavyAttackUp[2] * 2, 0, 0},
						duration = prepAttackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 1, frameSizeHeavyAttackUp[2] * 2, 0, 0},
						duration = attackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 2, frameSizeHeavyAttackUp[2] * 2, 0, 0},
						duration = attackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 3, frameSizeHeavyAttackUp[2] * 2, 0, 0},
						duration = attackHeavy1Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 0, frameSizeHeavyAttackUp[2] * 3, 0, 0},
						duration = attackHeavy1Dur / 4
					}
				},
				wholeFrameSize = frameSizeHeavyAttackUp,
				wholeDuration = 0.6,
				flipX = false,
				endToStart = false,
				isRepeating = false,
        transformOrigin = upAttackOrigin

    },
    {
				name = "Heavy_Attack_Up_2",
				textureLocation = TextureDir .. "human_back_attack.png",
				origLists = {
          {
						frame = {frameSizeHeavyAttackUp[1] * 1, frameSizeHeavyAttackUp[2] * 3, 0, 0},
						duration = prepAttackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 2, frameSizeHeavyAttackUp[2] * 3, 0, 0},
						duration = prepAttackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 3, frameSizeHeavyAttackUp[2] * 3, 0, 0},
						duration = prepAttackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 0, frameSizeHeavyAttackUp[2] * 4, 0, 0},
						duration = prepAttackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 1, frameSizeHeavyAttackUp[2] * 4, 0, 0},
						duration = attackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 2, frameSizeHeavyAttackUp[2] * 4, 0, 0},
						duration = attackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 3, frameSizeHeavyAttackUp[2] * 4, 0, 0},
						duration = attackHeavy2Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 0, frameSizeHeavyAttackUp[2] * 5, 0, 0},
						duration = attackHeavy2Dur / 4
					}
				},
				wholeFrameSize = frameSizeHeavyAttackUp,
				wholeDuration = 0.6,
				flipX = false,
				endToStart = false,
				isRepeating = false,
        transformOrigin = upAttackOrigin

    },
    {
				name = "Heavy_Attack_Up_3",
				textureLocation = TextureDir .. "human_back_attack.png",
				origLists = {
          {
						frame = {frameSizeHeavyAttackUp[1] * 1, frameSizeHeavyAttackUp[2] * 5, 0, 0},
						duration = prepAttackHeavy3Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 2, frameSizeHeavyAttackUp[2] * 5, 0, 0},
						duration = prepAttackHeavy3Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 3, frameSizeHeavyAttackUp[2] * 5, 0, 0},
						duration = prepAttackHeavy3Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 0, frameSizeHeavyAttackUp[2] * 6, 0, 0},
						duration = prepAttackHeavy3Dur / 4
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 1, frameSizeHeavyAttackUp[2] * 6, 0, 0},
						duration = attackHeavy3Dur / 5
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 2, frameSizeHeavyAttackUp[2] * 6, 0, 0},
						duration = attackHeavy3Dur / 5
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 3, frameSizeHeavyAttackUp[2] * 6, 0, 0},
						duration = attackHeavy3Dur / 5
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 0, frameSizeHeavyAttackUp[2] * 7, 0, 0},
						duration = attackHeavy3Dur / 5
					},
					{
						frame = {frameSizeHeavyAttackUp[1] * 1, frameSizeHeavyAttackUp[2] * 7, 0, 0},
						duration = attackHeavy3Dur / 5
					}
				},
				wholeFrameSize = frameSizeHeavyAttackUp,
				wholeDuration = 0.6,
				flipX = false,
				endToStart = false,
				isRepeating = false,
        transformOrigin = upAttackOrigin

    }
    
  
  
}