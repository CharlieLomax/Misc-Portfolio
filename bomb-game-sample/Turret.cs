using Godot;
using System;

public class Turret : Area2D
{
    private Player _target;
    
    [Export]
    private PackedScene bullet;

    [Export]
    private float fireSpeed;

    [Export]
    /** Maximum angle on either side of the target path the bullet's trajectory might be offset by */
    private float jitterAngle = 0;

    [Export]
    /** Half-angle of the cone the turret will target the player within */
    private float targetAngle = Mathf.Tau;

    [Export]
    /** Whether this turret will fire at the ship's current position or its predicted position */
    private bool leadsShots = false;

    [Export]
    private float rateOfFire = 0.3f;
    
    private Position2D _barrel;
    
    [Export]
    private float rotationRate = 40;

    private float _defeaultRotation;

    private Vector2 _defaultAim;

    [Export]
    private bool movingBase = false;

    private RigidBody2D _turretBase;

    private Vector2 _firePath;

    public override void _Ready()
    {
        GetNode<Timer>("RateOfFire").WaitTime = rateOfFire;
        _barrel = GetNode<Position2D>("TurretCollider/BarrelPoint");
        _defeaultRotation = Rotation;
        _defaultAim = Vector2.Up.Rotated(_defeaultRotation);

        if (movingBase)
        {
            _turretBase = GetParent<RigidBody2D>();
        }
    }

    public override void _Process(float delta)
    {
        if (_target != null)
        {
            _firePath = GetTargetPath();

            float angle = _defaultAim.AngleTo(_firePath);

            if (_firePath.AngleTo(_defaultAim) <= targetAngle / 2 && angle <= targetAngle / 2)
            {
                Rotation = Mathf.LerpAngle(Rotation, _defeaultRotation + angle, rotationRate * delta);
            }
        }
    }

    public void FireBullet()
    {
        if (_target != null)
        {
            // Determine the path to the turret's target
            //_firePath = GetTargetPath();
            
            if (_firePath.AngleTo(_defaultAim) <= targetAngle / 2 && _defaultAim.AngleTo(_firePath) <= targetAngle / 2)
            {
                // Randomly jitter the bullet's path
                float offset = (float)GD.RandRange(-jitterAngle, jitterAngle);
                _firePath = _firePath.Rotated(offset);

                // Create and fire the bullet
                Bullet shot = bullet.Instance<Bullet>();
                shot.GlobalPosition = _barrel.GlobalPosition;
                shot.Rotation = Vector2.Up.AngleTo(_firePath);

                if (movingBase)
                {
                    shot.LinearVelocity = _turretBase.LinearVelocity;
                }

                Vector2 fireVelocity = fireSpeed * _firePath.Normalized();

                shot.Fire(fireVelocity);
                GetTree().Root.CallDeferred("add_child", shot);
            }
        }
    }

    private Vector2 GetTargetPath()
    {
        
        Vector2 lineToTarget = _target.GlobalPosition - GlobalPosition;

        if (!leadsShots || _target.LinearVelocity.Length() == 0)
        {
            return lineToTarget;
        }

        // Calculate target path to intersect with the player based on current velocity
        // From https://www.codeproject.com/Articles/990452/Interception-of-Two-Moving-Objects-in-D-Space
        else
        {
            Vector2 targetVelocity = _target.LinearVelocity;

            if (movingBase)
            {
                targetVelocity -= _turretBase.LinearVelocity;
            }

            double targetSpeed = targetVelocity.Length();
            
            // Now set up the quadratic formula coefficients
            double a = fireSpeed * fireSpeed - targetSpeed * targetSpeed;
            double b = 2 * (-lineToTarget).Dot(targetVelocity);
            double c = -lineToTarget.LengthSquared();

            double t1, t2;
            if (!QuadraticSolver(a, b, c, out t1, out t2))
            {
                // No real-valued solution, so no interception possible
                return lineToTarget;
            }
            
            if (t1 < 0 && t2 < 0)
            {
                // Both values for t are negative, so the interception would have to have
                // occured in the past
                return lineToTarget;
            }

            double timeToInterception;

            if (t1 > 0 && t2 > 0) // Both are positive, take the smaller one
                timeToInterception = Math.Min(t1, t2);
            else // One has to be negative, so take the larger one
                timeToInterception = Math.Max(t1, t2);

            Vector2 interceptionPosition = _target.GlobalPosition + ((float)timeToInterception * targetVelocity);

            return interceptionPosition - GlobalPosition;
        }
    }

    /// <summary>
    /// Solve a quadratic equation in the form ax^2 + bx + c = 0. From https://www.codeproject.com/Articles/990452/Interception-of-Two-Moving-Objects-in-D-Space.
    /// </summary>
    /// <param name="a">Coefficient for x^2</param>
    /// <param name="b">Coefficient for x</param>
    /// <param name="c">Constant</param>
    /// <param name="solution1">The first solution</param>
    /// <param name="solution2">The second solution</param>
    /// <returns>TRUE if a solution exists, FALSE if one does not</returns>
    private static bool QuadraticSolver(double a, double b, double c, out double solution1, out double solution2)
    {
        if (a == 0)
        {
            if (b == 0)
            {
                solution1 = solution2 = double.NaN;
                return false;
            }
            else
              {
                  solution1 = solution2 = -c / b;
                  return true;
              }
        }
        double tmp = b * b - 4 * a * c;
        if (tmp < 0)
          {
              solution1 = solution2 = double.NaN;
              return false;
          }
        tmp = Math.Sqrt(tmp);
        double _2a = 2 * a;
        solution1 = (-b + tmp) / _2a;
        solution2 = (-b - tmp) / _2a;
        return true;
    }

    public void OnTargetRangeBodyEntered(PhysicsBody2D body)
    {
        if(body is Player p)
        {
            _target = p;
        }
    }

    public void OnTargetRangeBodyExited(PhysicsBody2D body)
    {
        if (body is Player p && p == _target)
        {
            _target = null;
        }
    }
}
